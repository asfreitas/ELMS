const mongoose = require('mongoose');

const Schema = mongoose.Schema;

const vehicleSchema = new Schema({
    vehicle_unit: { type: Number, required: true, unique: true},
    startup_time: { type: Date, required: true},  
    last_received_time: { type: Date, required: true},
    last_longitude: { type: Number, required: true},
    last_latitude: { type: Number, required: true},
    past_velocity: { type: Array, required: true},
    past_bearing: { type: Array, required: true},
    new_time: { type: Date, required: true},
    new_longitude: { type: Number, required: true}, 
    new_latitude: { type: Number, required: true},
    new_velocity: { type: Number, required: true}, 
    new_bearing: { type: Number, required: true},
    status: { type: String, required: true},
    distance_to_vehicles: [{ 
        vehicle_unit: { type: Number, required: true},
        distance: { type: Number,  required: true} 
    }]
},  {
    timestamps: true,
});


const Vehicle = mongoose.model('Vehicle', vehicleSchema);

module.exports = Vehicle;