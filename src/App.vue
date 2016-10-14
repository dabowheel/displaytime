<template>
  <div id="app">
    <settings v-bind:datetime="datetime" v-bind:date-format="dateFormat" v-bind:time-format="timeFormat" v-bind:date-formatted="dateFormatted" v-bind:time-formatted="timeFormatted"></settings>
    <p id="date"><big>{{ dateFormatted }}</big></p>
    <p id="time"><big>{{ timeFormatted }}</big></p>
  </div>
</template>

<script>
import moment from 'moment';
import Settings from './Settings.vue';

export default {
  data () {
    var datetime = moment();
    var dateFormat = localStorage.dateFormat ? localStorage.dateFormat: "dddd MMMM Do YYYY";
    var timeFormat = localStorage.timeFormat ? localStorage.timeFormat: "hh:mm";
    var dateFormatted = datetime.format(dateFormat);
    var timeFormatted = datetime.format(timeFormat);
    return {
      datetime: datetime,
      dateFormat: dateFormat,
      timeFormat: timeFormat,
      dateFormatted: dateFormatted,
      timeFormatted: timeFormatted
    }
  },
  watch: {
    datetime: function (newDatetime) {
      this.dateFormatted = newDatetime.format(this.dateFormat);
      this.timeFormatted = newDatetime.format(this.timeFormat);
    },
    dateFormat: function (newFormat) {
      this.dateFormatted = this.datetime(newFormat);
    },
    timeFormat: function (newFormat) {
      this.timeFormatted = this.datetime(newFormat);
    }
  },
  created() {
    setInterval(function () {
      this.datetime = moment();
    }.bind(this), 1000);
  },
  components: {
    Settings
  }
}
</script>

<style>
body {
  font-family: Helvetica, sans-serif;
  margin: 0;
  padding: 0;
}
#date,#time {
  text-align: center;
}
</style>
