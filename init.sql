BEGIN TRANSACTION;
create table user (email text, password text);
INSERT INTO user VALUES('miguel@here','abc');
COMMIT;
