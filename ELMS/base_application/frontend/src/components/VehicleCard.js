import React from "react"


function VehicleCard(props) {
//class VehicleCard extends React.Component{
    
    /*
    constructor(){
        super()
        this.state = {
            latitude: "",
            unit: "",
            longitude: "",
            active_time: "",
            at_risk_time: "",
            nearest_vehicle: ""
        }
    }
    */
    //render(){
        return(
            <div className="vehicle_status_column">
                <div className="vehicle_card">
                    <div className="latitude">
                        <p>Latitude</p>
                        {props.vehicle.latitude}
                    </div>
                    <div className="unit">
                        <p>Unit</p>
                        {props.vehicle.unit}
                    </div>
                    <div className="longitude">
                        <p>Longitude</p>
                        {props.vehicle.longitude}
                    </div>
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
        )
    //}
}

export default VehicleCard