import React, { Component } from 'react'
import axios from 'axios'

const Vehicle = props => (
        <tr>
            <td>{props.vehicle.vehicle_unit}</td>
            <td>{props.vehicle.pastVelocityAvg}</td>
            <td>{props.vehicle.startup_time}</td>
        </tr>

)
    

class Table extends Component {
    constructor(props) {
        super(props)
        this.state = {vehicles: []}
    }
    componentDidMount(){
        axios.get('http://localhost:8080/vehicles/analytics')
        .then(res => {
            this.setState({vehicles: res.data})
            console.log(res.data);
        })
        
        .catch((err) => {
            console.log(err);
        })
    }


     renderTableData() {
        console.log(this.state.vehicles);
        return this.state.vehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getAverageSpeed={this.getAverageSpeed} startup_time={this.getActiveTime} key={currentVehicle._id}/>;
        })
     }

  
     render() {
        return (
           <div>
            <table id='vehicles'>
                <thead>
                    <tr>
                        <th>Vehicle #</th>
                        <th>Average Speed</th>
                        <th>Total Time</th>
                    </tr>
                    </thead>
                <tbody>
                    {this.renderTableData()}
                </tbody>
            </table>
           </div>
        )
     }
}
export default Table //exporting a component make it reusable and this is the beauty of react