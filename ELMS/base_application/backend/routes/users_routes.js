/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the various routes that are used to 
* manipulate and get data from the users database.
*/


const router = require('express').Router();
let User = require('../models/user.model.js');

//router to get users from mongodb atlas
router.route('/').get((req, res) => {
    User.find()
    .then(users => res.json(users))
    .catch(err => res.status(400).json('Error: ' + err));
});

router.route('/add').post((req, res) => {
    const username = req.body.username;
    const password = req.body.password;
    const first_name = req.body.first_name;
    const last_name = req.body.last_name;
    const email = req.body.email;

    const newUser = new User({
        username,
        password,
        first_name,
        last_name,
        email,
    });

    newUser.save()
    .then(() => res.json('User added'))
    .catch(err => res.status(400).json('Error: ' + err));
});

module.exports = router;