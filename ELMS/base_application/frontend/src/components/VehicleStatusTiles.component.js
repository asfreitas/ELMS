import React from "react"
import VehicleRow from "./VehicleRow.component"
import VehicleAtRisk from "./VehicleAtRisk.component"
import VehicleActive from "./VehicleActive.component"
import VehicleInactive from "./VehicleInactive.component"

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