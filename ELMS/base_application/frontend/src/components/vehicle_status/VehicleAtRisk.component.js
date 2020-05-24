/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for displaying the vehicle at risk tiles.
*/

import React, { Component } from "react"
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"


export default class VehicleAtRisk extends VehicleBase{
    constructor(props){
        super(props);
        this.getAtRiskVehicles = this.getAtRiskVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.getAtRiskTime = this.getAtRiskTime.bind(this);
        this.state={vehicles: []}
        this.vehicleQuery = 'http://localhost:8080/vehicles/at_risk'

    }

    //get list of at risk vehicles
    getAtRiskVehicles(){
        return this.state.vehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} typeClass="active_time" typeText="Active time: " riskClass="at_risk_time"
            isAtRisk={true} riskText="At risk time: " getActiveTime={this.getActiveTime} getAtRiskTime={this.getAtRiskTime} key={currentVehicle._id}/>;
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