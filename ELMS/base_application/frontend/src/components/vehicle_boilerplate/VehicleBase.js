import React, { Component } from "react"
import axios from 'axios'
import Vehicle from "./VehicleProp.js"


export default class VehicleBase extends Component{
    constructor(props){
        super(props);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: [], nearest_vehicle: 0}
        this.vehicleQuery = 'http://localhost:8080/vehicles'
    }
    //automatically get vehicle list and parse by status
    
    componentDidMount() {
        this.getData()
        this.interval = setInterval(() => (this.setState({time: Date.now()}), 1000));

    }
    getData = () => {
        axios.get(this.vehicleQuery)
        .then(res => {
            this.setState({vehicles: res.data})
            console.log(res.data);
            this.intervalID = setTimeout(this.getData.bind(this), 5000);

        })
        
        .catch((err) => {
            console.log(err);
        })
    }
    componentWillUnmount(){
        clearInterval(this.interval);
        clearTimeout(this.timeout);
    }

    getActiveTime(vehicle){
        var date1 = vehicle.startup_time;
        //Convert dates into javascript readable date format and
        //get difference between dates/times in milliseconds
        var diff_in_milliseconds = Math.abs(new Date(date1) - new Date(Date.now()));
        
        //Get milliseconds, seconds, minutes, and hours
        var milliseconds = parseInt((diff_in_milliseconds % 1000) / 100);
        var seconds = Math.floor((diff_in_milliseconds / 1000) % 60);
        var minutes = Math.floor((diff_in_milliseconds / (1000 * 60)) % 60);
        var hours = Math.floor((diff_in_milliseconds / (1000 * 60 * 60)) % 24);

        //To format single digit hours with a "0" 
        hours = (hours < 10) ? "0" + hours : hours;
        minutes = (minutes < 10) ? "0" + minutes : minutes;
        seconds = (seconds < 10) ? "0" + seconds : seconds;

        return hours + ":" + minutes + ":" + seconds;
    }
    //set nearest vehicle
    getNearestVehicle(vehicle){
        var nearest = 0;
        var closest_distance = 10000;
        var total_close_vehicles = vehicle.distance_to_vehicles.length;
        for(var j = 0; j < total_close_vehicles; j++){
            if(vehicle.distance_to_vehicles[j].distance < closest_distance){
                closest_distance = vehicle.distance_to_vehicles[j].distance;
                nearest = vehicle.distance_to_vehicles[j].vehicle_unit;
            }
        }
        return nearest;
    }

}