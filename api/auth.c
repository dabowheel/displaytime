#include <aqua/aqua.h>
#include <sqlite3.h>
#include <randomid.h>
#include <sys/time.h>
#include "com.h"
#include "util.h"

/*
    CheckEmailQuery (PRIVATE)
    DESCRIPTION: Check if email exists
    INPUT:
        email - email address
    OUTPUT:
        CheckEmailQuery - the query string or NULL if error
        *errorptr - error string if error
    MEMORY:
        (if CheckEmailQuery (+ EmailQuery)) 
        (if (not CheckEmailQuery) (+ *errorptr))
*/
a_string CheckEmailQuery(a_string email, a_string *errorptr)
{
    a_string queryFormat;
    a_string query;

    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    queryFormat = a_cstr2s("SELECT id FROM user WHERE email = ?;");
    query = a_sqlformat(queryFormat, errorptr, email);
    a_sdestroy(queryFormat);

    /* (if (not query) (| *errorptr *errorptr) (return)) */
    if (!query) {
        return NULL;
    }

    /* (if query (| query EmailQuery) (return)) */
    return query;
}

/*
    NewUserQuery (PRIVATE)
    DESCRIPTION: Build query string for creating a new user
    INPUT:
        email - email address
        password - password
    OUTPUT:
        NewUserQuery - query to create new user if no error
        *errorptr - error if error
    MEMORY:
        (if NewUserQuery (+ NewUserQuery))
        (if (not NewUserQuery) (+ *errorptr))
*/
a_string NewUserQuery(a_string email, a_string password, a_string *errorptr)
{
    a_string queryFormat;
    a_string query;

    /* (+ queryFormat) */
    queryFormat = a_cstr2s("INSERT INTO user(email, password) VALUES (?, ?);");

    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    query = a_sqlformat(queryFormat, errorptr, email, password);

    /* (- queryFormat) */
    a_sdestroy(queryFormat);

    /* (if (not query) (| *errorptr *errorptr) (return)) */    
    if (!query) {
        return NULL;
    }

    /* (| query NewUserQuery) (return) */
    return query;
}

/*
    newSessionQuery (PRIVATE)
    DESCRIPTION: create a query to create a new session
    INPUT:
        id - the user ID
    OUTPUT:
        *sessionIDptr - session ID
        *expireptr - expiration datetime
        *errorptr - error if there is an error
        newSessionQuery - query or NULL if error
    MEMORY:
        newSessionQuery: + *sessionIDptr, *expireptr, newSessionQuery
        !newSessionQuery: + *errorptr
*/
a_string newSessionQuery(sqlite3_int64 id, a_string *sessionIDptr, a_string *expireptr, a_string *errorptr)
{
    a_string query;
    a_string query2;
    struct timeval t;
    int ret;
    a_string sessionID;
    a_string userID;
    a_string expire;
    a_string error;
    charmap map;

    /* sessionID: + sessionID */
    /* !sessionID: + *errorptr AND newSessionQuery GETS NULL AND END */
    map = Create62CharMap();
    sessionID = randomID(30, map, &error);
    cmDestroy(map);
    if (!sessionID) {
        *errorptr = error;
        return NULL;
    }

    /* + userID */
    userID = a_itoa(id);

    /* ret: + expire */
    /* !ret: + *errorptr AND getSessionQuery GETS NULL AND END */
    ret = gettimeofday(&t, NULL);
    if (ret) {
        *errorptr = a_cstr2s("Could not get time of day.");
        return NULL;
    }
    t.tv_sec += 60 * 60 * 24 * 7;       /* 60 s/m * 60 m/h * 24 h/d * 7 = seconds/day * 7 */
    expire = a_GetISOTime(&t);

    /* - userID */
    /* query2: + query2 */
    /* !query2: + errorptr AND getSessionQuery GETS NULL AND END */
    query = a_cstr2s("INSERT INTO session(id, userID, expire) VALUES (?, ?, ?);");
    query2 = a_sqlformat(query, &error, sessionID, userID, expire);
    a_sdestroy(query);
    a_sdestroy(userID);
    if (!query2) {
        *errorptr = error;
        return NULL;
    }

    /* sessionID | *sessionIDptr */
    /* expire | *expireptr */
    /* query2 | newSessionQuery */
    *sessionIDptr = sessionID;
    *expireptr = expire;
    return query2;
}

/*
    signup (PUBLIC)
    DESCRIPTION: handle signup request
    INPUT:
        req - request info
        body - request body
    OUTPUT:
        signup - response
    MEMORY:
        + signup
*/
response signup(request req, a_string body)
{

    response res;
    a_hash_table table;
    a_string email;
    a_string password;
    a_string query;
    a_string error2;
    sqlite3 *db;
    int rc;
    const char *error;
    char *error3;
    sqlite3_int64 id;
    a_string sessionID;
    a_string expire;
    sqlite3_stmt *stmt;
    int done;
    int has_row;
    a_string emailKey;
    a_string passwordKey;

    /* (+ table email|table password|table) */
    table = a_decodeForm(body);
    emailKey = a_cstr2s("email");
    passwordKey = a_cstr2s("password");
    email = a_htGet(table, emailKey);
    password = a_htGet(table, passwordKey);
    a_sdestroy(emailKey);
    a_sdestroy(passwordKey);

    /* !rc: +signup AND END */
    rc = sqlite3_open("displaytime.db", &db);
    if (rc) {
        error = sqlite3_errmsg(db);
        sqlite3_close(db);
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/html"));
        a_sbldaddcstr(res->body, "Error opening database: ");
        a_sbldaddcstr(res->body, error);
        return res;
    }

    /* (+ query) */
    query = CheckEmailQuery(email, &error2);
    rc = sqlite3_prepare(db, query->data, query->len + 1, &stmt, NULL);

    /* (- query) */
    a_sdestroy(query);

    /* (if rc (+signup) (return)) */
    if (rc) {
        error = sqlite3_errmsg(db);
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/html"));
        a_sbldaddcstr(res->body, "Error preparing query: ");
        a_sbldaddcstr(res->body, error);
        /* (| res signup) (return) */
        return res;
    }

    done = 0;
    has_row = 0;
    while (!done) {
        rc = sqlite3_step(stmt);
        switch(rc) {
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
                error = sqlite3_errmsg(db);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
                a_sbldaddcstr(res->body, "Error stepping through SQL results: ");
                a_sbldaddcstr(res->body, error);
                return res;
                break;
        }
    }

    sqlite3_finalize(stmt);

    /* (if has_row (+ signup) (return)) */
    if (has_row) {
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(200, a_cstr2s("OK"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "emailExists=true");
        /* (| res signup) (return) */
        return res;
    }

    /* (if query (+ query)) */
    /* (if (not query (+ error2)) */
    query = NewUserQuery(email, password, &error2);
    
    /* (if (not query) (- error2) (+ signup) (return)) */
    if (!query) {
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plan"));
        a_sbldadds(res->body, error2);
        /* (- error2) */
        a_sdestroy(error2);
        /* (| res signup) */
        return res;
    }

    /* (if rc (+ error3)) */
    rc = sqlite3_exec(db, query->data, NULL, NULL, &error3);

    /* (if rc (- error3) (+ signup) (return)) */
    if (rc) {
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/html"));
        a_sbldaddcstr(res->body, "Error executing database statement: ");
        a_sbldaddcstr(res->body, error3);
        /* (- error3) */
        sqlite3_free(error3);
        /* (| res signup) (return) */
        return res;
    }

    /* (- query) */
    a_sdestroy(query);

    id = sqlite3_last_insert_rowid(db);

    /* (if query (+ query sessionID expire)) */
    /* (if (not query) (+ error2)) */
    query = newSessionQuery(id, &sessionID, &expire, &error2);

    /* (if (not query) (- error2) (+ signup) (return)) */
    if (!query) {
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/html"));
        a_sbldaddcstr(res->body, "Error creating session: ");
        a_sbldadds(res->body, error2);
        /* (- error2 sessionID expire) */
        a_sdestroy(error2);
        a_sdestroy(sessionID);
        a_sdestroy(expire);

        /* (| res signup) (return) */
        return res;
    }

    /* (if rc (+ error3)) */
    rc = sqlite3_exec(db, query->data, NULL, NULL, &error3);

    /* (- query) */
    a_sdestroy(query);

    /* (if rc (- error3) (+signup) (return)) */
    if (rc) {
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/html"));
        a_sbldaddcstr(res->body, "Error creating session: ");
        a_sbldaddcstr(res->body, error3);
        /* (- error3 sessionID expire) */
        sqlite3_free(error3);
        a_sdestroy(sessionID);
        a_sdestroy(expire);
        /* (| res signup) (return) */
        return res;
    }

    sqlite3_close(db);

    /* (+ res) */
    res = createResponse(200, a_cstr2s("OK"), a_cstr2s("application/x-www-form-urlencoded"));
    a_sbldaddcstr(res->body, "emailExists=");
    a_sbldaddcstr(res->body, "&sessionID=");
    a_sbldaddcstr(res->body, sessionID->data);
    a_sbldaddcstr(res->body, "&sessionExpire=");
    a_sbldaddcstr(res->body, expire->data);

    /* (- sessionID, expire) */
    a_sdestroy(sessionID);
    a_sdestroy(expire);

    /* (| res, signup) */
    return res;
}

/*
    GetLoginQuery (PRIVATE)
    DESCRIPTION: Get a query string for checking login credentials
    INPUT:
        email - email
        password - password
    OUTPUT:
        GetLoginQuery - the query string or NULL if error
        *errorptr - unset or the error string if error
    MEMORY:
        (if GetLoginQuery (+ GetLoginQuery))
        (if (not GetLoginQuery) (+ *errorptr))
*/
a_string GetLoginQuery(a_string email, a_string password, a_string *errorptr)
{
    a_string queryFormat;
    a_string query;

    /* (if query (+ query)) */
    /* (if (not query) (+ *errorptr)) */
    queryFormat = a_cstr2s("SELECT id FROM user WHERE email = ? AND password = ?;");
    query = a_sqlformat(queryFormat, errorptr, email, password);
    a_sdestroy(queryFormat);

    /* (if (not query) (| *errorptr *errorptr) (return)) */
    if (!query) {
        return NULL;
    }

    /* (| query GetLoginQuery) */
    return query;
}

/*
    handleLogin (PRIVATE)
    handle login request
    INPUT:
        req - the request
        body - the request body
    OUTPUT:
        handleLogin - response
    MEMORY:
        (+ handleLogin)
*/
response handleLogin(request req, a_string body)
{
    a_string query;
    a_string error;
    const char *dberror;
    char *dberror2;
    response res;
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    int done;
    sqlite3_int64 id;
    int row_returned;
    a_string sessionID;
    a_string expire;
    a_string rc_num;

    a_string emailKey;
    a_string passwordKey;
    a_string email;
    a_string password;
    a_hash_table table;

    /* +table */
    emailKey = a_cstr2s("email");
    passwordKey = a_cstr2s("password");
    table = a_decodeForm(body);
    email = a_htGet(table, emailKey);
    password = a_htGet(table, passwordKey);
    a_sdestroy(emailKey);
    a_sdestroy(passwordKey);

    /* (if (or (not email) (not password)) (-table) (+ handleLogin) (return)) */
    if (!email || !password) {
        res = ApplicationError();
        a_sbldaddcstr(res->body, "email or password not present in body of request");
        a_htDestroy(table);
        return res;
    }

    /* (if query (+ query)) */
    /* (if (not query) (+ error)) */
    query = GetLoginQuery(email, password, &error);
    
    /* (- table) */
    a_htDestroy(table);

    /* (if (not query) (- error) (+ handleLogin) (return)) */
    if (!query) {
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "Error building login query: ");
        a_sbldadds(res->body, error);
        /* (- error) */
        a_sdestroy(error);
        /* (| res handleLogin) */
        return res;
    }

    rc = sqlite3_open("displaytime.db", &db);

    /* (if rc (-query) (+ handleLogin) (return)) */
    if (rc != SQLITE_OK) {
        dberror = sqlite3_errmsg(db);
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "Error opening database: ");
        rc_num = a_itoa(rc);
        a_sbldadds(res->body, rc_num);
        a_sdestroy(rc_num);
        a_sbldaddcstr(res->body, ": ");
        a_sbldaddcstr(res->body, dberror);
        /* (- query) */
        a_sdestroy(query);
        /* (| res handleLogin) */
        return res;
    }

    /* (if (not rc) (+stmt)) */
    rc = sqlite3_prepare(db, query->data, query->len + 1, &stmt, NULL);
    
    /* (if rc (+ handleLogin) (return)) */
    if (rc) {
        dberror = sqlite3_errmsg(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "Error preparing SQL statement: ");
        rc_num = a_itoa(rc);
        a_sbldadds(res->body, rc_num);
        a_sdestroy(rc_num);
        a_sbldaddcstr(res->body, ": ");
        a_sbldaddcstr(res->body, dberror);
        sqlite3_close(db);
        /* (| res handleLogin) */
        return res;
    }

    /* (- query) */
    a_sdestroy(query);

    done = 0;
    row_returned = 0;
    while (!done) {
        rc = sqlite3_step(stmt);
        switch (rc) {
            case SQLITE_BUSY:
                break;
            case SQLITE_ROW:
                id = sqlite3_column_int64(stmt, 0);
                row_returned = 1;
                done = 1;
                break;
            case SQLITE_DONE:
                done = 1;
                break;
            case SQLITE_ERROR:
            case SQLITE_MISUSE:
            default:
                dberror = sqlite3_errmsg(db);
                /* (- stmt) */
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                /* (+ res) */
                res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plan"));
                a_sbldaddcstr(res->body, "Error stepping though SQL statement: ");
                a_sbldaddcstr(res->body, dberror);
                /* (| res handleLogin) (return)*/
                return res;
        }
    }

    /* (- stmt) */
    sqlite3_finalize(stmt);

    /* (if (not row_returned) (+ handleLogin) (return)) */
    if (!row_returned) {
        sqlite3_close(db);
        /* (+ res) */
        res = createResponse(200, a_cstr2s("OK"), a_cstr2s("application/x-wwww-form-urlencoded"));
        a_sbldaddcstr(res->body, "success=");
        /* (| res handleLogin) */
        return res;
    }

    query = newSessionQuery(id, &sessionID, &expire, &error);

    if (!query) {
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldadds(res->body, error);
        a_sdestroy(error);
        return res;
    }

    rc = sqlite3_exec(db, query->data, NULL, NULL, &dberror2);

    /* (if rc (+ handleLogin) (return)) */
    if (rc) {
        sqlite3_close(db);
        dberror = sqlite3_errmsg(db);
        /* (+ res) */
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/plain"));
        a_sbldaddcstr(res->body, "Error executing SQL statement: ");
        a_sbldaddcstr(res->body, dberror2);
        sqlite3_free(dberror2);
        /* (| res handleLogin) */
        return res;
    }

    
    /* (- query) */
    a_sdestroy(query);

    sqlite3_close(db);
    
    /* (+ res) */
    res = createResponse(200, a_cstr2s("OK"), a_cstr2s("application/x-www-form-urlencoded"));
    a_sbldaddcstr(res->body, "success=true&sessionID=");
    a_sbldaddcstr(res->body, sessionID->data);
    a_sbldaddcstr(res->body, "&sessionExpire=");
    a_sbldaddcstr(res->body, expire->data);
    
    writeLog(expire->data);

    /* (- sessionID expire) */
    a_sdestroy(sessionID);
    a_sdestroy(expire);

    /* (| res handleLogin) */
    return res;
}

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
