import React, { Component } from "react";
import Grid from "@material-ui/core/Grid";
import axios from "axios";

class home extends Component {
  state = {
   weather:null
  };
  componentDidMount() {
    axios
      .get("/Weather_Data")
      .then((res) => {

        var dataweather = res.data;
        console.log( res.data);
              this.setState({
         weather:dataweather
        });
        // for (var i = 0; i < weatherkeys.length; i++) {
        //   var k = weatherkeys[i];
        //   var temp = dataweather[k];
        //   var humidity = dataweather[k].Humidity;
        //   //addhumidity = addhumidity+parseInt(humidity);
        //   console.log(temp, humidity);

        //    this.setState({
        //  temperature : temp,
        //  humidity:humidity
        // });
        // }

        // console.log(res.data);
        // this.setState({
        //   weather: res.data,
        // });
      })
      .catch((err) => console.log(err));
  }

  render() {
    let recentweather = this.state.humidity ? (
      this.state.weather.map((weatherdata) => <p>{weatherdata.Temperature}</p>)
    ) : (
      <p> Loading</p>
    );
    return (
      <Grid container>
        <Grid item sm={8} xs={12}>
          {recentweather}
        </Grid>
      </Grid>
    );
  }
}

export default home;
