import React, { Component } from "react"
//import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import VehicleBase from "../vehicle_boilerplate/VehicleBase"

const Vehicle = props => (
    <div className="vehicle_status_column">
        <div className="vehicle_card">
            <div className="latitude">
                <p>Latitude</p>
                {props.vehicle.new_latitude}
            </div>
            <div className="unit">
                <p>Unit</p>
                {props.vehicle.vehicle_unit}
            </div>
            <div className="longitude">
                <p>Longitude</p>
                {props.vehicle.new_longitude}
            </div>
            <div className="active_time">
                Active time: 
                {props.getActiveTime(props.vehicle)}
            </div>
            <div className="nearest_vehicle">
                Nearest vehicle: 
                {props.getNearestVehicle(props.vehicle)}
            </div>
        </div>
    </div>
)

export default class VehicleActive extends VehicleBase{
    constructor(props){
        super(props);
        this.getActiveVehicles = this.getActiveVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: []}
        this.vehicleQuery = 'http://localhost:8080/vehicles/active'
    }
    //get list of inactive vehicles
    getActiveVehicles(){
        return this.state.vehicles.map(currentVehicle => {
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