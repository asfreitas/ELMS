const Express = require('express');
const Path = require(`path`);   //used for getting path of files
const cors = require('cors');
//environment variables in .env file
require('dotenv').config();
const mongoose = require('mongoose');

const uri = process.env.ATLAS_URI;
const DATABASE_NAME = "elms";

const app = Express();


app.use(cors());
//parse json
app.use(Express.json());

const port = 8080;

mongoose.connect(uri, {useUnifiedTopology: true, useNewUrlParser: true, useCreateIndex: true});
const connection = mongoose.connection;
connection.once('open', () => {
    console.log("MongoDB database connection established successfully");
})

const vehicleRouter = require('./routes/vehicles_routes.js');
const userRouter = require('./routes/users_routes.js');

app.use('/vehicles', vehicleRouter);
app.use('/users', userRouter);



app.listen(port, () => {
    console.log(`Server running on port: ${port}`);
});
