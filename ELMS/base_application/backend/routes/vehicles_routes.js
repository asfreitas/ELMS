const router = require('express').Router();
let Vehicles = require('../models/vehicle.model.js');


//Mongoose method to get list of all vehicles from mongodb atlas db
router.route('/').get((req, res) => {
    Vehicles.find()
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
    const status = req.body.status;

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
        status,
        distance_to_vehicles,
    });

    new_vehicle.save()
    .then(() => res.json('Vehicle added'))
    .catch(err => res.status(400).json('Error: ' + err));
});

//Get vehicle by id
router.route('/:id').get((req, res) => {
    Vehicles.findById(req.params.id)
    .then(vehicle => res.json(vehicle))
    .catch(err => res.status(400).json('Error: ' + err));
});

//Delete vehicle by id
router.route('/:id').delete((req, res) => {
    Vehicles.findByIdAndDelete(req.params.id)
    .then(() => res.json('Vehicle Deleted'))
    .catch(err => res.status(400).json('Error: ' + err));
});


router.route('/update/:id').post((req, res) => {
    Vehicles.findById(req.params.id)
    .then(vehicle => {
        vehicle.vehicle_unit = req.body.vehicle_unit;
        vehicle.startup_time = Date.parse(req.body.startup_time);
        vehicle.last_received_time = Date.parse(req.body.last_received_time);
        vehicle.last_longitude = req.body.last_longitude;
        vehicle.last_latitude = req.body.last_latitude;
        vehicle.past_velocity = req.body.past_velocity; 
        vehicle.past_bearing = req.body.past_bearing;
        vehicle.new_time = Date.parse(req.body.new_time);
        vehicle.new_longitude = req.body.new_longitude;
        vehicle.new_latitude = req.body.new_latitude;
        vehicle.new_velocity = req.body.new_velocity;
        vehicle.new_bearing = req.body.new_bearing;
        vehicle.distance_to_vehicles = req.body.distance_to_vehicles;
        vehicle.status = req.body.status;
        
        vehicle.save()
            .then(() => res.json('Vehicle updated'))
            .catch(err => res.status(400).json('Error: ' + err));

    })
    .catch(err => res.status(400).json('Error: ' + err));
})


module.exports = router;