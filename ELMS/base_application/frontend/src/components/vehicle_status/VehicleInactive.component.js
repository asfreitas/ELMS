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
    }

    //get list of inactive vehicles
    getInactiveVehicles(){
        const vehicle_list_length = this.state.vehicles.length;
        var inactiveVehicles = [];
        var j = 0;
        for(var i = 0; i < vehicle_list_length; i++){
            if(this.state.vehicles[i].status === "inactive"){
                inactiveVehicles[j] = this.state.vehicles[i];
                j++;
            }
        }
        console.log(inactiveVehicles);
        return inactiveVehicles.map(currentVehicle => {
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

