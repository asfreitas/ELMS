import React, {Component} from "react"
import Vehicle from "../vehicle_boilerplate/VehicleProp.js"
import axios from 'axios'
import VehicleBase from "../vehicle_boilerplate/VehicleBase"


export default class OfflineVehicle extends VehicleBase{
    constructor(props){
        super(props);
        this.getOfflineVehicles = this.getOfflineVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: [], nearest_vehicle: 0}
        this.vehicleType = 'http://localhost:8080/vehicles/offline_vehicles/'
    }

    //automatically get vehicle list and parse by status
    getOfflineVehicles(){
      //  console.log(this.state.vehicles);
        return this.state.vehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} getActiveTime={this.getActiveTime} key={this.state.vehicles._id}/>;
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

