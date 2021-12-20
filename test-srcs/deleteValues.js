const { Pool, Client } = require('pg')


const client = new Client({
    user: 'leet_admin',
    host: 'localhost',
    database: 'webserv',
    password: 'leetask_tst',
    port: 5432,
});


const execute = async (query) => {
    try {
        await client.connect();     // gets connection
        await client.query(query);  // sends queries
        return true;
    } catch (error) {
        console.error(error.stack);
        return false;
    } finally {
        await client.end();         // closes connection
    }
};

let text = `DELETE FROM USERS WHERE NAME='MAMOUSSA'`;

execute(text).then(result => {
    if (result) {
        console.log('DELETE VALUES');
    }
});