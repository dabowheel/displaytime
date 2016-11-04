#include <aqua/aqua.h>
#include <sqlite3.h>
#include <string.h>
#include "com.h"
#include "util.h"
#include "profile.h"
  
/*
    GetProfileQuery (PUBLIC)
    DESCRIPTION: get profile query
    INPUT:
        sessionID - session ID
    OUTPUT:
        GetProfileQuery - get profile query
        *errorptr - error if any
    MEMORY:
        (if GetProfileQuery (+GetProfileQuery))
        (if (not GetProfileQuery (+ *errorptr)))
*/
a_string GetProfileQuery(a_string sessionID, a_string *errorptr)
{
    a_string queryFormat;
    a_string query;

    /* (+ queryFormat) */
    queryFormat = a_cstr2s("SELECT user.id, user.email, user.password FROM session, user WHERE session.id = ? AND session.userID = user.id;");
    
    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    query = a_sqlformat(queryFormat, errorptr, sessionID);

    /* (if (not query) (| *errorptr *errorptr) (return)) */
    if (!query) {
        return NULL;
    }

    /* (| query GetProfileQuery) (return) */
    return query;
}

/*
    CreateProfile (PRIVATE)
    DESCRIPTION: Create a profile object
    INPUT:
        id - id
        email - email 
        password - password
    OUTPUT:
        CreateProfile - profile
    MEMORY:
        (+ CreateProfile)
*/
profile CreateProfile(char *id, char *email, char *password)
{
    profile p = a_malloc(sizeof(struct profile));
    p->id = id;
    p->email = email;
    p->password = password;
    return p;
}

void DestroyProfile(profile p)
{
    free(p->id);
    free(p->email);
    free(p->password);
    free(p);
}

/*
    GetProfile (PRIVATE)
    DESCRIPTION: Get a profile from a session ID
    INPUT:
        sessionID - session ID
    OUTPUT:
        GetProfile - profile if no error
        *errorptr - error if any
    MEMORY:
        (if GetProfile (+ GetProfile))
        (if (not GetProfile) (+ *errorptr))
*/
profile GetProfile(sqlite3 *db, a_string sessionID, a_string *errorptr)
{
    a_string_builder b;
    a_string error;
    profile p;
    a_string query;
    int rc;
    const char *dberror;
    sqlite3_stmt *stmt;
    int done;
    int has_row;
    a_string userID;
    a_string email;
    a_string password;

    /* (if query (+ query)) */
    /* (if (not query) (error)) */
    query = GetProfileQuery(sessionID, &error);
    writeLog(query->data);

    /* (if (not query) (- error) (+ GetProfile) (return)) */
    if (!query) {
        /* (+ b) */
        b = a_sbldcreate();
        a_sbldaddcstr(b, "Error building get profile query: ");
        a_sbldadds(b, error);
        /* (- error) */
        a_sdestroy(error);
        /* (| b *errorptr) (return)*/
        *errorptr = a_sbld2s(b);
        return NULL;
    }

    rc = sqlite3_open("displaytime.db", &db);

    /* (if rc (- query) (+ *errorptr) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        sqlite3_close(db);
        /* (+ b) */
        b = a_sbldcreate();
        a_sbldaddcstr(b, "Error opening connect to db: ");
        a_sbldaddcstr(b, dberror);
        /* (- query) */
        a_sdestroy(query);
        /* (| b *errorptr) (return) */
        *errorptr = a_sbld2s(b);
        return NULL;
    }

    rc = sqlite3_prepare(db, query->data, query->len + 1, &stmt, NULL);

    /* (if rc (+ *errorptr) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        /* (+ b) */
        b = a_sbldcreate();
        a_sbldaddcstr(b, "Error preparing SQL statement: ");
        a_sbldaddcstr(b, dberror);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        /* (| b *errorptr) */
        *errorptr = a_sbld2s(b);
        return NULL;
    }

    /* (if (= rc SQLITE_ERROR SQLITEMISUSE) (+ *errorptr) (return)) */
    done = 0;
    has_row = 0;
    while (!done) {
        rc = sqlite3_step(stmt);
        switch (rc) {
            case SQLITE_BUSY:
                break;
            case SQLITE_DONE:
                done = 1;
                break;
            case SQLITE_ROW:
                has_row = 1;
                done = 1;
                break;
            case SQLITE_ERROR:
            case SQLITE_MISUSE:
            default:
                dberror = sqlite3_errmsg(db);
                /* (b) */
                b = a_sbldcreate();
                a_sbldaddcstr(b, dberror);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                /* (| b *errorptr) (return) */
                *errorptr = a_sbld2s(b);
                return NULL;
        }
    }

    /* (if (not has_row) (+ *errorptr) (return)) */
    if (!has_row) {
        /* (+ b) */
        b = a_sbldcreate();
        a_sbldaddcstr(b, "Profile not found");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        /* (| b *errorptr) (return) */
        *errorptr = a_sbld2s(b);
        return NULL;
    }

    /* (+ userID email password) */
    userID = a_cstr2s((char*)sqlite3_column_text(stmt, 0));
    email = a_cstr2s((char*)sqlite3_column_text(stmt, 1));
    password = a_cstr2s((char*)sqlite3_column_text(stmt, 2));

    /* (+ p) (| userID p) (| email p) (| password p) */
    p = CreateProfile(a_s2cstr(userID), a_s2cstr(email), a_s2cstr(password));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    /* (| p GetProfile) (return) */
    return p;
}

response HandleGetProfile(request req, a_string body)
{
    a_string sessionID;
    a_hash_table table;
    a_string sessionIDKey;
    response res;
    profile p;
    sqlite3 *db;
    a_string error;
    int rc;
    const char *dberror;

    /* (+ table) */
    table = a_decodeForm(req->query_string);
    writeLog(req->query_string->data);

    /* (+ sessionIDKey) */
    sessionIDKey = a_cstr2s("sessionID");
    sessionID = a_htGet(table, sessionIDKey);

    /* (- sessionIDKey) */
    a_sdestroy(sessionIDKey);

    /* (if (not sessionID) (+ HandleGetProfile) (return)) */
    if (!sessionID) {
        /* (+ res) */
        res = ApplicationErrorDescription("sessionID not found in body of request");
        /* (| res HandleGetProfile) */
        return res;
    }

    /* (+ db) */
    rc = sqlite3_open("displaytime.db", &db);

    /* (if rc (- db) (+ HandleGetProfile) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        /* (+ res) */
        res = ApplicationErrorDetails("Count not open connection to database: ", dberror);
        /* (- db) */
        sqlite3_close(db);
        /* (| res HandleGetProfile) (return) */
        return res;
    }

    /* (if p (+ p)) */
    /* (if (not p) (+ error)) */
    p = GetProfile(db, sessionID, &error);

    /* (if (not p) (-db) (+HandleGetProfile) (return)) */
    if (!p) {
        /* (+ res) */
        res = ApplicationErrorDetails("Could not get profile: ", error->data);

        /* (- error) */
        a_sdestroy(error);

        /* (- db) */
        sqlite3_close(db);

        /* (| res HandleGetProfile) (return) */
        return res;
    }

    /* (+ res) */
    res = FormResponse();
    a_sbldaddcstr(res->body, "email=");
    a_sbldaddcstr(res->body, p->email);

    /* (-p) */
    DestroyProfile(p);

    /* (- db) */
    sqlite3_close(db);

    /* (| res HandleGetProfile) */
    return res;
}

/*
    FormatSet (PUBLIC)
    DESCRIPTION: format the SET part of an update query for a single parameter
    INPUT:
        key - key
        value - value
        *countptr - count of parameters
    OUTPUT:
        FormatSet - formatted string if no error
        *countptr - count of parameters
        *errorptr - error if any
    MEMORY:
        (if FormatSet (+ FormatSet))
        (if (not FormatSet) (+ errorptr))
*/
a_string FormatSet(a_string key, a_string value, int *countptr, a_string *errorptr)
{
    a_string_builder b;
    a_string queryFormat;
    a_string query;

    /* (+ b) */
    b = a_sbldcreate();
    if (*countptr)
        a_sbldaddchar(b, ',');
    a_sbldaddchar(b, ' ');
    a_sbldadds(b, key);
    a_sbldaddcstr(b, " = ?");

    /* (- b) */
    /* (+ queryFormat) */
    queryFormat = a_sbld2s(b);

    /* (if query (+query)) */
    /* (if (not query) (+ *errorptr)) */
    query = a_sqlformat(queryFormat, errorptr, value);
    (*countptr)++;

    /* (- queryFormat) */
    a_sdestroy(queryFormat);

    /* (if query (| query FormatSet) */
    /* (if (not query) (| *errorptr *errorptr)) */
    /* (return) */
    return query;
}

/*
    ProfileUpdateQuery (PRIVATE)
    DESCRIPTION: build query to update the user profile
    INPUT:
        sessionID - session ID
        table - hash table of one or more request variables: email, password
    OUTPUT:
        ProfileUpdateQuery - query to update the user profile if no error
        *errorptr - error if any
    MEMEORY:
        (if ProfileUpdateQuery (+ ProfileUpdateQuery))
        (if (not ProfileUpdateQuery) (+ *errorptr))
*/
a_string ProfileUpdateQuery(a_string userID, a_hash_table table, a_string *errorptr)
{
    a_string queryFormat;
    a_string query;
    a_string email;
    a_string password;
    a_string newPassword;
    a_string_builder b;
    a_string emailKey;
    a_string passwordKey;
    a_string newPasswordKey;
    int count = 0;

    /* (+ b) */
    b = a_sbldcreate();
    a_sbldaddcstr(b, "UPDATE user SET");


    /* (+ emailKey) */
    emailKey = a_cstr2s("email");
    email = a_htGet(table, emailKey);

    /* (if email (if (not query) (+ *errorptr) (return))) */
    if (email) {
        /* (if query (+ query)) */
        /* (if (not query) (+ *errorptr)) */
        query = FormatSet(emailKey, email, &count, errorptr);

        /* (if (not query) (- emailKey) (+errorptr) (return)) */
        if (!query) {
            /* (- emailKey) */
            a_sdestroy(emailKey);
            /* (| *errorptr *errorptr) */
            return NULL;
        }
        a_sbldadds(b, query);

        /* (- query) */
        a_sdestroy(query);
    }

    /* (- emailKey) */
    a_sdestroy(emailKey);

    /* (+ newPasswordKey) */
    newPasswordKey = a_cstr2s("newPassword");
    newPassword = a_htGet(table, newPasswordKey);

    /* (+ passwordKey) */
    passwordKey = a_cstr2s("password");
    password = a_htGet(table, passwordKey);

    /* (if (newPassword) (if query (+ *errorptr) (return))) */
    if (newPassword) {
        /* (+ query) */
        query = FormatSet(passwordKey, newPassword, &count, errorptr);
        /* (if query (+ *errorptr) (return)) */
        if (!query) {
            /* (- passwordKey) */
            a_sdestroy(newPasswordKey);
            /* (| *errorptr *errorptr) */
            return NULL;
        }
        a_sbldadds(b, query);

        /* (- query) */
        a_sdestroy(query);
    }

    /* (- newPasswordKey) */
    a_sdestroy(newPasswordKey);

    /* (+ queryFormat) */
    queryFormat = a_cstr2s(" WHERE id = ? AND password = ?;");

    /* (- passwordKey) */
    a_sdestroy(passwordKey);

    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    query = a_sqlformat(queryFormat, errorptr, userID, password);

    /* (- queryFormat) */
    a_sdestroy(queryFormat);

    /* (if (not query) (+ *errorptr) (return)) */
    if (!query) {
        return NULL;
    }

    a_sbldadds(b, query);
    
    /* (- query) */
    a_sdestroy(query);

    /* (| b ProfileUpdateQuery) (return) */
    return a_sbld2s(b);
}

/*
    UpdateProfile (PRIVATE)
    DESCRIPTION: update profile
    INPUT:
        db - database
        sessionID - user ID
        table - table
    OUTPUT:
        UpdateProfile - 1 if successful, otherwise 0
        *errorptr - error if not successful
    MEMORY:
        (if (not UpdateProfile) (+ *errorptr))
*/
int UpdateProfile(sqlite3 *db, a_string userID, a_hash_table table, a_string *errorptr)
{
    int rc;
    const char *dberror;
    a_string_builder b;
    a_string query;
    sqlite3_stmt *stmt;
    int done;
    int count;

    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    query = ProfileUpdateQuery(userID, table, errorptr);
    writeLog("query");
    writeLog(query->data);

    /* (if (not query) (+ *errorptr) (return)) */
    if (!query) {
        /* (| *errorptr *errorptr) */
        return 0;
    }

    /* (+ stmt) */
    rc = sqlite3_prepare(db, query->data, query->len + 1, &stmt, NULL);

    /* (- query) */
    a_sdestroy(query);

    /* (if rc (- stmt) (+ *errorptr) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        /* (+ res) */
        b = a_sbldcreate();
        a_sbldaddcstr(b, dberror);
        /* (- stmt) */
        sqlite3_finalize(stmt);
        /* (| b *errorptr) */
        *errorptr = a_sbld2s(b);
        /* (return) */
        return 0;
    }

    /* (if (= rc SQLITE_ERROR SQLITE_MISUSE) (+ *errorptr) (return)) */
    done = 0;
    while (!done) {
        rc = sqlite3_step(stmt);
        switch (rc) {
            case SQLITE_BUSY:
                break;
            case SQLITE_DONE:
            case SQLITE_ROW:
                done = 1;
                break;
            case SQLITE_ERROR:
            case SQLITE_MISUSE:
            default:
                dberror = sqlite3_errmsg(db);
                /* (+ b) */
                b = a_sbldcreate();
                a_sbldaddcstr(b, dberror);
                sqlite3_finalize(stmt);
                /* (| b *errorptr) */
                *errorptr = a_sbld2s(b);
                /* (return) */
                return 0;
        }
    }

    count = sqlite3_changes(db);

    /* (- stmt) */
    sqlite3_finalize(stmt);

    /* (if (< count 1) (+ HandleUpdateProfile)) */
    if (count < 1) {
        /* (+ b) */
        b = a_sbldcreate();
        a_sbldaddcstr(b, "Could not find profile.");

        /* (| b *errorptr) */
        *errorptr = a_sbld2s(b);

        /* (return) */
        return 0;
    }

    return 1;
}

/*
    HandleUpdateProfile (PUBLIC)
    DESCRIPTION: update profile
    INPUT:
        req - request
        body - body of request
    OUTPUT:
        HandleUpdateProfile - response
    MEMORY:
        (+ HandleUpdateProfile)
*/
response HandleUpdateProfile(request req, a_string body)
{
    a_hash_table bodyTable;
    a_string sessionID;
    a_string userID;
    a_string sessionIDKey;
    response res;
    int rc;
    sqlite3 *db;
    const char *dberror;
    a_hash_table urlTable;
    a_string error;
    profile p;
    a_string password;
    a_string passwordKey;
    a_string newPasswordKey;
    a_string newPassword;

    /* (+ db) */
    rc = sqlite3_open("displaytime.db", &db);

    /* (if res (- db) (+ HandleUpdateProfile) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        /* (- db) */
        sqlite3_close(db);
        /* (+ res) */
        res = ApplicationErrorDescription("Error when opening database: ");
        a_sbldaddcstr(res->body, dberror);
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

    /* (+ urlTable) */
    urlTable = a_decodeForm(req->query_string);

    /* (+ sessionIDKey) */
    sessionIDKey = a_cstr2s("sessionID");
    sessionID = a_htGet(urlTable, sessionIDKey);

    /* (- sessionIDKey) */
    a_sdestroy(sessionIDKey);

    /* (if (not sessionID) (+ HandleUpdateProfile) (return)) */
    if (!sessionID) {
        /* (- urlTable) */
        a_htDestroy(urlTable);
        /* (+ res) */
        res = ApplicationErrorDescription("sessionID was not found in the body of the request");
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

    /* (if p (+p)) */
    /* (if (not p) (+ error)) */
    p = GetProfile(db, sessionID, &error);

    /* (- urlTable) */
    a_htDestroy(urlTable);

    /* (if (not p) (+ HandleUpdateProfile) (return)) */
    if (!p) {
        /* (+ res) */
        res = ApplicationErrorDetails("Could not find profile: ", error->data);
        /* (- error) */
        a_sdestroy(error);
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

    /* (+ bodyTable) */
    bodyTable = a_decodeForm(body);

    /* (+ newPasswordKey) */
    newPasswordKey = a_cstr2s("newPassword");

    /* (if newPassword (+ newPassword)) */
    newPassword = a_htGet(bodyTable, newPasswordKey);

    /* (- newPasswordKey) */
    a_sdestroy(newPasswordKey);

    if (newPassword && newPassword->len > 0) {
        /* (+ passwordKey) */
        passwordKey = a_cstr2s("password");

        password = a_htGet(bodyTable, passwordKey);
        a_sdestroy(passwordKey);
 
        /* (if (not password) (+ HandleUpdateProfile) (return)) */
        if (!password || password->len < 1) {
            /* (+ res) */
            res = ApplicationErrorDescription("Old password must be specified to set a new password");
            /* (- bodyTable p) */
            a_htDestroy(bodyTable);
            DestroyProfile(p);
            /* (| res HandleUpdateProfile) (return) */
            return res;
        }

        /* (if password doesn't match (- bodyTable p) (+ HandleUpdateProfile) (return)) */
        if (strcmp(p->password, password->data) != 0) {
            /* (- bodyTable p) */
            a_htDestroy(bodyTable);
            DestroyProfile(p);
            /* (+ res) */
            res = ApplicationErrorDescription("Old Password does not match current password");
            /* (| res HandleUpdateProfile) */
            return res;
        }
    }

    /* (+ userID) */
    userID = a_cstr2s(p->id);

    /* (if (not UpdateProfile) (- bodyTable) (+ HandleUpdateProfile) (return)) */
    if (!UpdateProfile(db, userID, bodyTable, &error)) {
        /* (- bodyTable userID p) */
        a_htDestroy(bodyTable);
        a_sdestroy(userID);
        DestroyProfile(p);
        /* (+ res) */
        res = ApplicationErrorDetails("Could not update profile: ", error->data);
        /* (- error) */
        a_sdestroy(error);
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

    /* (- bodyTable userID p) */
    a_htDestroy(bodyTable);
    a_sdestroy(userID);
    DestroyProfile(p);

    /* (+ res) */
    res = FormResponse();
    /* (| res HandleUpdateProfile) (return) */
    return res;
}
