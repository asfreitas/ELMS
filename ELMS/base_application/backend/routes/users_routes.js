/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the various routes that are used to 
* manipulate and get data from the users database while utilizing
* jwt tokens.
*/
const router = require('express').Router();
const bcrypt = require('bcryptjs');
const config = require('config');
const jwt = require('jsonwebtoken');
const User = require('../models/user.model.js');
const auth = require('../middleware/auth');

//router to get users from mongodb atlas
router.route('/').get((req, res) => {
    User.find()
    .select('-password')
    .then(users => res.json(users))
    .catch(err => res.status(400).json('Error: ' + err));
});

//add user information added to mongodb with hash password using jwts
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
    //create salt & hash
    bcrypt.genSalt(10, (err, salt) => {
        if(err) throw err;
        bcrypt.hash(newUser.password, salt, (err, hash) => {
            if(err) throw err;
            newUser.password = hash;
            newUser.save()
            .then((user) => {
                //create token associated with userid
                jwt.sign(
                    { id: user.id }, 
                    config.get('jwtSecret'),
                    { expiresIn: 3600 },
                    (err, token) => {
                        if(err) throw err;
                        //response with jwt token and some of user info
                        res.json({
                            token, 
                            user: {
                                id: user.id,
                                first_name: user.first_name,
                                last_name: user.last_name,
                                email: user.email
                            }
                        })
                    }
                )
            })
            .catch(err => res.status(400).json('Error: ' + err));
        });
    });
});


//router to authenticate user
router.route('/auth').post((req, res) => {
    const username = req.body.username;
    const password = req.body.password;

    User.findOne({username})
    .then(user => {
        if(!user) return res.status(400).json({msg: "User does not exst"});

        //validate password
        bcrypt.compare(password, user.password)
        .then(isMatch => {
            if(!isMatch) return res.status(400).json({msg: "Invalid username or password"});  
            jwt.sign(
                { id: user.id }, 
                config.get('jwtSecret'),
                { expiresIn: 3600 },
                (err, token) => {
                    if(err) throw err;
                    //response with jwt token and some of user info
                    res.json({
                        token, 
                        user: {
                            id: user.id,
                            first_name: user.first_name,
                            last_name: user.last_name,
                            email: user.email
                        }
                    })
                }
            )
        })
    })
});

//get user data using jwt
router.route('/auth/user').get(auth, (req, res) => {
    User.findById(req.user.id)
    .select('-password')
    .then(user => res.json(user));
});


module.exports = router;