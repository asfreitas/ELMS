import React, { Component } from "react"

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
            <div className="at_risk_time">
                At risk time: 
                {props.vehicle.at_risk_time}
            </div>
            <div className="nearest_vehicle">
                Nearest vehicle: 
                {props.getNearestVehicle(props.vehicle)}
            </div>
        </div>
    </div>
)
    
export default Vehicle;
