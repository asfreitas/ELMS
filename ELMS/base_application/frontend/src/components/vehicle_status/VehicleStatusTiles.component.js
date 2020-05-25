/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the structure for displaying the 
* vehicle status page.
*/

import React from "react"
import VehicleRow from "./VehicleRow.component"
import VehicleAtRisk from "./VehicleAtRisk.component"
import VehicleActive from "./VehicleActive.component"
import VehicleInactive from "./VehicleInactive.component"

//Formats the vehicle status page. Takes in the vehicles at risk, active, and inactive
//components and returns the order of rendering. 
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