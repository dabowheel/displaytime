#include <aqua/aqua.h>
#include <sqlite3.h>
#include "com.h"
#include "util.h"
  
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
    queryFormat = a_cstr2s("SELECT user.email FROM session, user WHERE session.id = ? AND session.userID = user.id;");
    
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

response HandleGetProfile(request req, a_string body)
{
    a_string query;
    a_string sessionID;
    a_string error;
    a_hash_table table;
    a_string sessionIDKey;
    response res;
    int rc;
    sqlite3 *db;
    const char *dberror;
    sqlite3_stmt *stmt;
    int done;
    int has_row;
    const char *emailstr;

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
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "sessionID not found in body of request");
        /* (| res HandleGetProfile) */
        return res;
    }

    /* (if query (+ query)) */
    /* (if (not query) (*errorptr)) */
    query = GetProfileQuery(sessionID, &error);

    /* (if (not query) (- error table) (+ HandleGetProfile) (return)) */
    if (!query) {
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "Error building get profile query: ");
        a_sbldadds(res->body, error);
        /* (- error table) */
        a_sdestroy(error);
        a_htDestroy(table);
        /* (| res HandleGetProfile) (return)*/
        return res;
    }

    rc = sqlite3_open("displaytime.db", &db);

    /* (if rc (- query table) (+ HandleGetProfile) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "Error opening connect to db: ");
        a_sbldaddcstr(res->body, dberror);
        /* (- query table) */
        a_sdestroy(query);
        a_htDestroy(table);
        /* (| res HandleGetProfile) (return) */
        return res;
    }

    rc = sqlite3_prepare(db, query->data, query->len + 1, &stmt, NULL);

    /* (if rc (+HandleGetProfile) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "Error preparing SQL statement: ");
        a_sbldaddcstr(res->body, dberror);
        /* (| res HandleGetProfile) */
        return res;
    }

    /* (if dberror (+ HandleGetProfile) (return)) */
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
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                /* (+ res) */
                res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
                a_sbldaddcstr(res->body, dberror);
                /* (| res HandleGetProfile) */
                return res;
        }
    }

    /* (if (not has_row) (+ HandleGetProfile) (return)) */
    if (!has_row) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        /* (+ res) */
        res = ApplicationError();
        a_sbldaddcstr(res->body, "Profile not found");
        /* (| res HandleGetProfile) (return) */
        return res;
    }

    emailstr = (char*)sqlite3_column_text(stmt, 0);
    /* (+res) */
    res = FormResponse();
    a_sbldaddcstr(res->body, "email=");
    a_sbldaddcstr(res->body, emailstr);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    /* (| res HandleGetProfile) (return) */
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
a_string ProfileUpdateQuery(a_string sessionID, a_hash_table table, a_string *errorptr)
{
    a_string queryFormat;
    a_string query;
    a_string email;
    a_string password;
    a_string_builder b;
    a_string emailKey;
    a_string passwordKey;
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

    /* (+ passwordKey) */
    passwordKey = a_cstr2s("password");
    password = a_htGet(table, passwordKey);

    /* (if (password) (if query (+ *errorptr) (return))) */
    if (password) {
        /* (+ query) */
        query = FormatSet(passwordKey, password, &count, errorptr);
        /* (if query (+ *errorptr) (return)) */
        if (!query) {
            /* (- passwordKey) */
            a_sdestroy(passwordKey);
            /* (| *errorptr *errorptr) */
            return NULL;
        }
        a_sbldadds(b, query);

        /* (- query) */
        a_sdestroy(query);
    }

    /* (- passwordKey) */
    a_sdestroy(passwordKey);

    /* (+ queryFormat) */
    queryFormat = a_cstr2s(" WHERE id = (SELECT userID FROM session WHERE id = ?);");

    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    query = a_sqlformat(queryFormat, errorptr, sessionID);

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
    a_hash_table table;
    a_string sessionID;
    a_string sessionIDKey;
    response res;
    a_string query;
    a_string error;
    int rc;
    sqlite3 *db;
    const char *dberror;
    sqlite3_stmt *stmt;
    int done;
    int count;

    /* (+ table) */
    table = a_decodeForm(body);

    /* (+ sessionIDKey) */
    sessionIDKey = a_cstr2s("sessionID");
    sessionID = a_htGet(table, sessionIDKey);

    /* (- sessionIDKey) */
    a_sdestroy(sessionIDKey);

    /* (if (not sessionID) (- table) (+ HandleUpdateProfile) (return)) */
    if (!sessionID) {
        /* (- table) */
        a_htDestroy(table);
        /* (+ res) */
        res = ApplicationErrorDescription("sessionID was not found in the body of the request");
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    query = ProfileUpdateQuery(sessionID, table, &error);

    /* (if (not query) (- table) (+ HandleUpdateProfile) (return)) */
    if (!query) {
        /* (- table) */
        a_htDestroy(table);
        /* (+ res) */
        res = ApplicationErrorDetails("Error building user ID query: ", error);
        /* (- error) */
        a_sdestroy(error);
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

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

    rc = sqlite3_prepare(db, query->data, query->len + 1, &stmt, NULL);

    /* (if rc (+ HandleUpdateProfile) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        sqlite3_close(db);
        /* (+ res) */
        res = ApplicationErrorDescription("Error while preparing query: ");
        a_sbldaddcstr(res->body, dberror);
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

    /* (if [sql error] (+ HandleUpdateProfile) (return)) */
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
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                /* (+ res) */
                res = ApplicationErrorDescription("Error stepping though query results: ");
                a_sbldaddcstr(res->body, dberror);
                /* (| res HandleUpdateProfile) (return) */
                return res;
        }
    }

    count = sqlite3_changes(db);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    /* (if (< count 1) (+ HandleUpdateProfile)) */
    if (count < 1) {
        /* (+res) */
        res = ApplicationErrorDescription("Could not find profile.");
        /* (| res HandleUpdateProfile) (return) */
        return res;
    }

    /* (+ res) */
    res = FormResponse();

    /* (| res HandleUpdateProfile) (return) */
    return res;
}
