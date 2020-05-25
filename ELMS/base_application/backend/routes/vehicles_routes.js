/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the various routes necessary for getting 
* data for use with displaying vehicles.
*/


const router = require('express').Router();
let Vehicles = require('../models/vehicle.model.js');


//get all vehicles in the database
router.route('/').get((req, res) => {
    Vehicles.find()
    .then(vehicles => res.json(vehicles))
    .catch(err => res.status(400).json('Error: ' + err));
});

// get all vehicles whose status is offline
router.route('/offline').get((req, res) => {
    Vehicles.find({"status" : "offline"})
    .then(vehicles => res.json(vehicles))
    .catch(err => res.status(400).json('Error: ' + err));
});

// get all vehicles whose status is at risk
router.route('/at_risk').get((req, res) => {
    Vehicles.find({"status" : "at_risk"})
    .then(vehicles => res.json(vehicles))
    .catch(err => res.status(400).json('Error: ' + err));
});

// get all vehicles whose status is active
router.route('/active').get((req, res) => {
    Vehicles.find({"status" : "active"})
    .then(vehicles => res.json(vehicles))
    .catch(err => res.status(400).json('Error: ' + err));
});

// get all vehicles whose status is inactive
router.route('/inactive').get((req, res) => {
    Vehicles.find({"status" : "inactive"})
    .then(vehicles => res.json(vehicles))
    .catch(err => res.status(400).json('Error: ' + err));
});

// get all vehicles whose status is inactive
router.route('/deleteall').delete((req, res) => {
    Vehicles.remove({})
    .then(vehicles => res.json(vehicles))
    .catch(err => res.status(400).json('Error: ' + err));
});
/*
* get all vehicle_unit, average all velocities and set to pastVelocityAvg,
* time the vehicle has been running (in milliseconds), and the startup time
* formatted to year-month-day
*/
router.route('/analytics').get((req, res) => {
    Vehicles.aggregate([
            {
            $project: {
                vehicle_unit: 1,
                pastVelocityAvg: {$round: [{$divide: [{$sum: "$past_velocity"}, {$size: "$past_velocity"}]}, 1]},
                time_alive: {$subtract: ["$$NOW", "$startup_time"]},
                startup_time: {"$dateToString": {date: "$startup_time", format: "%Y-%m-%d"}}
        }
    }])
    .then(vehicles => res.json(vehicles))
    .catch(err => res.status(400).json('Error: ' + err));
});

//handles post request saves to mongo db
router.route('/add').post((req, res) => {
    const vehicle_unit = req.body.vehicle_unit;
    const startup_time = Date.parse(req.body.startup_time);
    const last_received_time = Date.parse(req.body.last_received_time);
    const last_longitude = req.body.last_longitude;
    const last_latitude = req.body.last_latitude;
    const past_velocity = req.body.past_velocity; 
    const past_bearing = req.body.past_bearing;
    const new_time = Date.parse(req.body.new_time);
    const new_longitude = req.body.new_longitude;
    const new_latitude = req.body.new_latitude;
    const new_velocity = req.body.new_velocity;
    const new_bearing = req.body.new_bearing;
    const distance_to_vehicles = req.body.distance_to_vehicles;

    const new_vehicle = new Vehicles({
        vehicle_unit,
        startup_time,
        last_received_time,
        last_longitude,
        last_latitude,
        past_velocity,
        past_bearing,
        new_time,
        new_longitude,
        new_latitude,
        new_velocity,
        new_bearing,
        distance_to_vehicles,
    });

    new_vehicle.save()
    .then(() => res.json('Vehicle added'))
    .catch(err => res.status(400).json('Error: ' + err));
});



module.exports = router;