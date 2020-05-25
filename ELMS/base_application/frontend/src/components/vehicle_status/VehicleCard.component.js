/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for displaying the vehicle active tiles.
*/

import React from "react"

//function used to display a vehicles latitude, longitude, unit number, active time, at risk time, and nearest vehicle
function VehicleCard(props) {
        return(
            <div className="vehicle_status_column">
                <div className="vehicle_card">
                    <div className="latitude">
                        <p>Latitude</p>
                        {props.vehicle.latitude}
                    </div>
                    <div className="unit">
                        <p className="unit_text">Unit</p>
                        {props.vehicle.unit}
                    </div>
                    <div className="longitude">
                        <p className="longitude_text">Longitude</p>
                        {props.vehicle.longitude}
                    </div>
                    <div className="status_info">
                        <div className="active_time">
                            Active time: 
                            {props.vehicle.active_time}
                        </div>
                        <div className="at_risk_time">
                            At risk time: 
                            {props.vehicle.at_risk_time}
                        </div>
                        <div className="nearest_vehicle">
                            Nearest vehicle: 
                            {props.vehicle.nearest_vehicle}
                        </div>
                    </div>
                </div>
            </div>
        )
    //}
}

export default VehicleCard