/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for displaying the vehicle inactive tiles.
*/

import React, { Component } from "react"
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"

//class used to render inactive vehicles
export default class VehicleInactive extends VehicleBase{
    constructor(props){
        super(props)
        this.getInactiveVehicles = this.getInactiveVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getInactiveTime = this.getInactiveTime.bind(this);
        this.state={vehicles: []}
        this.vehicleQuery = 'https://elms-base-application.uc.r.appspot.com/vehicles/inactive'
    }

    //get list of inactive vehicles with nearest vehicle and inactive time
    getInactiveVehicles(){
        return this.state.vehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} getActiveTime={this.getInactiveTime} key={currentVehicle._id}
            isatRisk={false} typeClass="active_time" typeText="Inactive time: " />;
        })
    }

    render(){
        return(
            <div className="inactive">
                <hr className="inactive_line"/>
                    <p className="inactive_text">Inactive</p>
                <hr className="inactive_line"/>
                { this.getInactiveVehicles() }
            </div>
        )
    }
}