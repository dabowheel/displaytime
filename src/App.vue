<template>
  <div id="app">
    <p id="date">{{time.format("dddd MMMM Do YYYY")}}</p>
    <p id="time">{{time.format("hh:mm")}}</p>
  </div>
</template>

<script>
import moment from 'moment';
export default {
  data () {
    return {
      time: moment()
    }
  },
  created() {
    setInterval(function () {
      this.time = moment();
    }.bind(this), 1000);
  }
}
</script>

<style>
body {
  background-color: black;
  color: white;
  font-family: Helvetica, sans-serif;
}
p {
  text-align: center;
}
#date,#time {
  font-size: 60pt;
}
</style>
