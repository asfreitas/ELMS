import React, { Component } from "react"
import axios from 'axios'
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"


export default class VehicleActive extends VehicleBase{
    constructor(props){
        super(props);
        this.getActiveVehicles = this.getActiveVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: []}
    }
    //get list of inactive vehicles
    getActiveVehicles(){
        const vehicle_list_length = this.state.vehicles.length;
        var activeVehicles = [];
        var j = 0;
        for(var i = 0; i < vehicle_list_length; i++){
            if(this.state.vehicles[i].status === "active"){
                activeVehicles[j] = this.state.vehicles[i];
                j++;
            }
        }
        console.log(activeVehicles);
        return activeVehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} getActiveTime={this.getActiveTime} key={currentVehicle._id}/>;
        })
    }

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

