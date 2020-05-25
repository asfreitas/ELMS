/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the skeleton html code for a vehicle tile.
*/


import React from "react"

const Vehicle = props => (
    <div className="vehicle_status_column">
        <div className="vehicle_card">
            <div className="latitude">
                <p>Latitude</p>
                {Number(props.vehicle.new_latitude).toFixed(4)}
            </div>
            <div className="unit">
                <p>Unit</p>
                {props.vehicle.vehicle_unit}
            </div>
            <div className="longitude">
                <p>Longitude</p>
                {Number(props.vehicle.new_longitude).toFixed(4)}
            </div>
            <div className={props.typeClass}>
                {props.typeText}
                {props.getActiveTime(props.vehicle)}
            </div>
            {props.isAtRisk &&
            <div className={props.riskClass}>
                {props.riskText}
                {props.getAtRiskTime(props.vehicle)}
            </div>}
            <div className="nearest_vehicle">
                Nearest vehicle: 
                {props.getNearestVehicle(props.vehicle)}
            </div>
        </div>
    </div>
)



    
export default Vehicle;
