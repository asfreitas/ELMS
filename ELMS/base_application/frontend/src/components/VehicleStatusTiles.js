import React from "react"
import VehicleRow from "./VehicleRow"
import VehicleAtRisk from "./VehicleAtRisk"
import VehicleActive from "./VehicleActive"
import VehicleInactive from "./VehicleInactive"

function VehicleStatusTiles(){
    return(
        <div className="vehicle_status_tiles">
            <VehicleAtRisk />
            <VehicleRow />
            <VehicleActive />
            <VehicleRow />
            <VehicleInactive />
            <VehicleRow />
        </div>
    )
}

export default VehicleStatusTiles