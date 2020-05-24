/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the schema for users for use with 
* the web application. It could be used to enable permissions
* or other future functionality with the application.
*/


const mongoose = require('mongoose');

const Schema = mongoose.Schema;

const userSchema = new Schema({
    username: { type: String, required: true, unique: true, trim: true, minlength: 5},
    password: { type: String, required: true},
    first_name: { type: String, required: true},
    last_name: { type: String, required: true},
    email: { type: String, required: true, unique: true}
}, {
    timestamps: true,
});


const User = mongoose.model('User', userSchema);

module.exports = User;