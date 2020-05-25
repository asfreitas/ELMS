/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* Server for connecting to mongodb, and setting up routes used for app
*/

const Express = require('express');

const Path = require("path");   //used for getting path of files
const cors = require('cors');
//environment variables in .env file
require('dotenv').config();
const mongoose = require('mongoose');
const config = require('config');

//const uri = process.env.ATLAS_URI;
const uri = config.get('mongoURI');
const DATABASE_NAME = "elms";

const app = Express();


app.use(cors());
//parse json
app.use(Express.json());


const port = process.env.PORT || 8080;

mongoose.connect(uri, {useUnifiedTopology: true, useNewUrlParser: true, useCreateIndex: true});
const connection = mongoose.connection;
connection.once('open', () => {
    console.log("MongoDB database connection established successfully");
})

const vehicleRouter = require('./routes/vehicles_routes.js');
const userRouter = require('./routes/users_routes.js');


//serving build index.html file for deploying in Google App Engine
app.use(Express.static(Path.join(__dirname, "/build")));

app.use('/vehicles', vehicleRouter);
app.use('/users', userRouter);

//use catchall handler routes for getting react index.html
app.get("/*", (req, res) => {
    res.sendFile(Path.join(__dirname, "/build/index.html"));
});


app.listen(port, () => {
    console.log(`Server running on port: ${port}`);
});
