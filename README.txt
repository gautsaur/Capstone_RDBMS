Greetings.

I wanted to provide a few examples of how to test some commands that have been implemented and tested. Typing these commands will get the process going.
The commands are actual data that is stored in the directory I am submitting and will read real data.

Note, the commands must be terminated with a semi colon. If it is not, an error will be displayed.

The following database name is the one created: test

you can also list the databases by using the command: list database;


//
//We first want to open a Database. Do so like this:

open database test;

// The database is now open. You will know so by seeing the prompt "test@SQL>"
// You can list all tables in the database test by the command:

list tables;

// You can select from those tables. Use this command to select all columns from the test table:

select * from test_table;

// You can see the columns. Next try to select using specific columns like so:

select id from test_table;
select * from test_table;
select id,name from test_table;
select name,id from test_table;

// This is the most complete function. We have the guts and the parsing engine working now, and the rest of the commands should come up quick. Hope you have fun playing with this.