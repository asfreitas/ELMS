import React, { Component } from "react"
import axios from 'axios'
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"


export default class VehicleAtRisk extends VehicleBase{
    constructor(props){
        super(props);
        this.getAtRiskVehicles = this.getAtRiskVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: [], nearest_vehicle: 0}
    }

    //get list of at risk vehicles
    getAtRiskVehicles(){
        const vehicle_list_length = this.state.vehicles.length;
        var atRiskVehicles = [];
        var j = 0;
        for(var i = 0; i < vehicle_list_length; i++){
            if(this.state.vehicles[i].status === "at_risk"){
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
            <div className="at_risk">
                <hr className="at_risk_line"/>
                    <p className="at_risk_text">At Risk</p>
                <hr className="at_risk_line"/>
                { this.getAtRiskVehicles() }
            </div>
        )
    }
}

