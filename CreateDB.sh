#!/usr/bin/env bash
#
# Creates our database tables.

existing_user=`psql --tuples-only --quiet -c "select usename from pg_catalog.pg_user" | egrep webdemo`

if [ -n "$existing_user" ]; then
    echo "User $existing_user exists."
else
    echo createuser --createdb --pwprompt webdemo
    echo "Please enter webdemo-asdf as the password."
    echo
    createuser --createdb --pwprompt webdemo
fi
echo

existing_line=""
if [ -f ~/.pgpass ]; then
    existing_line=`egrep webdemo ~/.pgpass`
fi
if [ -n "$existing_line" ]; then
    echo ".pgpass has webdemo listed."
else
    echo "*::*:webdemo:webdemo-asdf" >> ~/.pgpass
    chmod og-rwx ~/.pgpass
    echo "Updated .pgpass"
fi
echo

existing_db=`psql --tuples-only --quiet -c "SELECT datname FROM pg_database" | egrep webdemo`

if [ -n "$existing_db" ]; then
    echo "Database $existing_db exists"
else
    echo createdb -U webdemo webdemo
    createdb -U webdemo webdemo
fi
echo

psql -U webdemo webdemo << EOF
CREATE TABLE IF NOT EXISTS member (id int primary key, username varchar(1024), crypted_password varchar(1024), name_first varchar(1024), name_list varchar(1024));
CREATE SEQUENCE IF NOT EXISTS member_seq START WITH 1;

CREATE TABLE IF NOT EXISTS event_template (id int primary key, name varchar(1024));
CREATE SEQUENCE IF NOT EXISTS event_template_seq START WITH 1;

CREATE TABLE IF NOT EXISTS event (id int primary key, name varchar(1024), event_template_id int references event_template(id) );
CREATE SEQUENCE IF NOT EXISTS event_seq START WITH 1;
EOF
