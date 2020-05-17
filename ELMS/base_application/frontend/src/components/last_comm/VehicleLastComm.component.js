import React, { Component } from "react"
import axios from 'axios'
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"


export default class OfflineVehicle extends VehicleBase{
    constructor(props){
        super(props);
        this.getOfflineVehicles = this.getOfflineVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: [], nearest_vehicle: 0}
    }
    //automatically get vehicle list and parse by status
    getOfflineVehicles(){
        const vehicle_list_length = this.state.vehicles.length;
        var atRiskVehicles = [];
        var j = 0;
        for(var i = 0; i < vehicle_list_length; i++){
            if(this.state.vehicles[i].status === "offline"){
                atRiskVehicles[j] = this.state.vehicles[i];
                j++;
            }
        }
        console.log(atRiskVehicles);
        return atRiskVehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} getActiveTime={this.getActiveTime} key={currentVehicle._id}/>;
        })
    }
    render(){
        return(
            <div className="offline_vehicles">
                <hr className="offline_vehicles_line"/>
                    <p className="offline_vehicles_text">Offline</p>
                <hr className="offline_vehicles_line"/>
                { this.getOfflineVehicles() }
            </div>
        )
    }
}

