import React from "react"
import VehicleCard from "./VehicleCard"


//This function will take in vehicle objects to send to
//the vehicle card to then display the vehicle info
function VehicleRow(){
    return(
        <div className="vehicle_status_rows">
            <VehicleCard 
                vehicle= {{unit: 1, latitude: 1234.1215, longitude: 3211.4125, active_time: "10:10:12", at_risk_time: "00:00:45", nearest_vehicle: 2}}
            />
            <VehicleCard 
                vehicle= {{unit: 2, latitude: 1234.1215, longitude: 3211.4125, active_time: "10:10:12", at_risk_time: "00:00:45", nearest_vehicle: 1}}
            />
            <VehicleCard 
                vehicle= {{unit: 3, latitude: 1234.1215, longitude: 3211.4125, active_time: "10:10:12", at_risk_time: "00:00:45", nearest_vehicle: 2}}
            />
            <VehicleCard 
                vehicle= {{unit: 4, latitude: 1234.1215, longitude: 3211.4125, active_time: "10:10:12", at_risk_time: "00:00:45", nearest_vehicle: 3}}
            />
        </div>
    )
}

export default VehicleRow 