/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for handling the add vehicle component.
*/

import React, { Component } from 'react';
import axios from 'axios'

import Navbar from '../Navbar.component'
import Footer from "../Footer.component.js"

//class used to add vehicle and information
export default class AddVehicle extends Component {
    constructor(props) {
        super(props);

        this.onChangeUnit = this.onChangeUnit.bind(this);
        this.onChangeStartTime = this.onChangeStartTime.bind(this);
        this.onChangeLastReceivedTime = this.onChangeLastReceivedTime.bind(this);
        this.onChangeLastLongitude = this.onChangeLastLongitude.bind(this);
        this.onChangeLastLatitude = this.onChangeLastLatitude.bind(this);
        this.onChangePastVelocity = this.onChangePastVelocity.bind(this);
        this.onChangePastBearing = this.onChangePastBearing.bind(this);
        this.onChangeNewTime = this.onChangeNewTime.bind(this);
        this.onChangenewLongitude = this.onChangenewLongitude.bind(this);
        this.onChangeNewLatitude = this.onChangeNewLatitude.bind(this);
        this.onChangeNewVelocity = this.onChangeNewVelocity.bind(this);
        this.onChangeNewBearing = this.onChangeNewBearing.bind(this);
        this.onChangeDistanceToVehicles = this.onChangeDistanceToVehicles.bind(this);
        this.onChangeStatus = this.onChangeStatus.bind(this);
        this.onSubmit = this.onSubmit.bind(this);

        this.state = {
            vehicle_unit: 0,
            startup_time: new Date(),
            last_received_time: new Date(),
            last_longitude: 0,
            last_latitude: 0,
            past_velocity: [], 
            past_bearing: [],
            new_time: new Date(),
            new_longitude: 0,
            new_latitude: 0,
            new_velocity: 0,
            new_bearing: 0,
            distance_to_vehicles: [],
            status: ""
        }
    }
    
    //form submit add unit number
    onChangeUnit(event){
        this.setState({
            vehicle_unit: event.target.value
        })
    }
    //form submit add unit number
    onChangeStartTime(event){
        this.setState({
            startup_time: event.target.value
        })
    }
    //form submit add unit number
    onChangeLastReceivedTime(event){
        this.setState({
            last_received_time: event.target.value
        })
    }
    //form submit add unit number
    onChangeLastLongitude(event){
        this.setState({
            last_longitude: event.target.value
        })
    }
    //form submit add unit number
    onChangeLastLatitude(event){
        this.setState({
            last_latitude: event.target.value
        })
    }
    //form submit add unit number
    onChangePastVelocity(event){
        this.setState({
            past_velocity: event.target.value
        })
    }
    //form submit add unit number
    onChangePastBearing(event){
        this.setState({
            past_bearing: event.target.value
        })
    }
    //form submit add unit number
    onChangeNewTime(event){
        this.setState({
            new_time: event.target.value
        })
    }
    //form submit add unit number
    onChangenewLongitude(event){
        this.setState({
            new_longitude: event.target.value
        })
    }
    //form submit add unit number
    onChangeNewLatitude(event){
        this.setState({
            new_latitude: event.target.value
        })
    }
    //form submit add unit number
    onChangeNewVelocity(event){
        this.setState({
            new_velocity: event.target.value
        })
    }
    //form submit add unit number
    onChangeNewBearing(event){
        this.setState({
            new_bearing: event.target.value
        })
    }
    //form submit add unit number
    onChangeDistanceToVehicles(event){
        this.setState({
            distance_to_vehicles: event.target.value
        })
    }
    //form submit add unit number
    onChangeStatus(event){
        this.setState({
            status: event.target.value
        })
    }

    //on submit form, create new vehicle object with input data
    onSubmit(event){
        event.preventDefault();

        const vehicle = {
            vehicle_unit: this.state.vehicle_unit,
            startup_time: this.state.startup_time,
            last_received_time: this.state.last_received_time,
            last_longitude: this.state.last_longitude,
            last_latitude: this.state.last_latitude,
            past_velocity: [1.1, 2.1, this.state.past_velocity], 
            past_bearing: [1.2, 1.5, this.state.past_bearing],
            new_time: this.state.new_time,
            new_longitude: this.state.new_longitude,
            new_latitude: this.state.new_latitude,
            new_velocity: this.state.new_velocity,
            new_bearing: this.state.new_bearing,
            distance_to_vehicles: [
                {
                    vehicle_unit: 1,
                    distance: 5
                },
                {
                    vehicle_unit: 12,
                    distance: this.state.distance_to_vehicles
                }
            ],
            status: this.state.status
        }

        console.log(vehicle);
        
        axios.post('https://elms-base-application.uc.r.appspot.com/vehicles/add', vehicle)
        .then(res => console.log(res.data));

        this.setState({
            vehicle_unit: 0,
            startup_time: new Date(),
            last_received_time: new Date(),
            last_longitude: 0,
            last_latitude: 0,
            past_velocity: [], 
            past_bearing: [],
            new_time: new Date(),
            new_longitude: 0,
            new_latitude: 0,
            new_velocity: 0,
            new_bearing: 0,
            distance_to_vehicles: [],
            status: ""
        })

    }

    //form used for creating a vehicle
    render(){
        return (
            <div>
                           <div>
                <h3>Create Vehicle</h3>
                <div class="addVehicle">
                    <form onSubmit={this.onSubmit}>
                        <div className="form-group">
                            <label>Vehicle unit: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.vehicle_unit}
                            onChange={this.onChangeUnit}
                            />
                        </div>
                        <div className="form-group">
                            <label>Startup Time: </label>
                            <input type="datetime-local"
                            required
                            className="form-control"
                            value={this.state.startup_time}
                            onChange={this.onChangeStartTime}
                            />
                        </div>
                        <div className="form-group">
                            <label>Last Received Time: </label>
                            <input type="datetime-local"
                            required
                            className="form-control"
                            value={this.state.last_received_time}
                            onChange={this.onChangeLastReceivedTime}
                            />
                        </div>
                        <div className="form-group">
                            <label>Last Longitude: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.last_longitude}
                            onChange={this.onChangeLastLongitude}
                            />
                        </div>
                        <div className="form-group">
                            <label>Last Latitude: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.last_latitude}
                            onChange={this.onChangeLastLatitude}
                            />
                        </div>
                        <div className="form-group">
                            <label>Past Velocity: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.past_velocity}
                            onChange={this.onChangePastVelocity}
                            />
                        </div>
                        <div className="form-group">
                            <label>Past Bearing: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.past_bearing}
                            onChange={this.onChangePastBearing}
                            />
                        </div>
                        <div className="form-group">
                            <label>New Time: </label>
                            <input type="datetime-local"
                            required
                            className="form-control"
                            value={this.state.new_time}
                            onChange={this.onChangeNewTime}
                            />
                        </div>
                        <div className="form-group">
                            <label>New Longitude: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.new_longitude}
                            onChange={this.onChangenewLongitude}
                            />
                        </div>
                        <div className="form-group">
                            <label>New Latitude: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.new_latitude}
                            onChange={this.onChangeNewLatitude}
                            />
                        </div>
                        <div className="form-group">
                            <label>New Velocity: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.new_velocity}
                            onChange={this.onChangeNewVelocity}
                            />
                        </div>
                        <div className="form-group">
                            <label>New Bearing: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.new_bearing}
                            onChange={this.onChangeNewBearing}
                            />
                        </div>
                        <div className="form-group">
                            <label>Distance to Vehicle 12: </label>
                            <input type="number"
                            required
                            className="form-control"
                            value={this.state.distance_to_vehicles}
                            onChange={this.onChangeDistanceToVehicles}
                            />
                        </div>
                        <div className="form-group">
                            <label>Status: </label>
                            <select ref="status"
                            required
                            className="form-control"
                            value={this.state.status}
                            onChange={this.onChangeStatus}>
                            <option >Select...</option>
                            <option value="at_risk">At risk</option>
                            <option value="active">Active</option>
                            <option value="inactive">Inactive</option>
                            <option value="offline">Offline</option>
                            </select>
                        </div>
                        <div class="form-button">
                            <button type="submit" value="Create Vehicle" class="btn btn-primary">Submit</button>

                        </div>
                    </form>

                    </div>
            </div>
            </div>
        )
    }
}