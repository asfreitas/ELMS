import React, { Component } from "react"
import axios from 'axios'
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"


export default class VehicleInactive extends VehicleBase{
    constructor(props){
        super(props)
        this.getInactiveVehicles = this.getInactiveVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: []}
        this.vehicleQuery = 'http://localhost:8080/vehicles/inactive'
    }

    //get list of inactive vehicles
    getInactiveVehicles(){
        return this.state.vehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} getActiveTime={this.getActiveTime} key={currentVehicle._id}/>;
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