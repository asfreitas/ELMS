/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for displaying the vehicle active tiles.
*/

import React, { Component } from "react"
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"

//class used to render active vehicles
export default class VehicleActive extends VehicleBase{
    constructor(props){
        super(props);
        this.getActiveVehicles = this.getActiveVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: []}
        this.vehicleQuery = 'http://localhost:8080/vehicles/active' // this will be used in the base class query
    }
    //return the list of active vehicles with: nearest vehicle, active time
    getActiveVehicles(){
        return this.state.vehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} isAtRisk={false}
            getActiveTime={this.getActiveTime} key={currentVehicle._id} typeClass="active_time" typeText="Active time: "/>;
        })
    }
    //Render the page with header of active and cards of vehicles active
    render(){
        return(
            <div className="active">
                <hr className="active_line"/>
                    <p className="active_text">Active</p>
                <hr className="active_line"/>
                { this.getActiveVehicles() }
            </div>
        )
    }
}