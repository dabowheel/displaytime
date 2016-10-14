<template>
  <div id="app">
    <settings></settings>
    <p id="date"><big>{{time.format("dddd MMMM Do YYYY")}}</big></p>
    <p id="time"><big>{{time.format("hh:mm")}}</big></p>
  </div>
</template>

<script>
import moment from 'moment';
import Settings from './Settings.vue';

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
  },
  components: {
    Settings
  }
}
</script>

<style>
body {
  font-family: Helvetica, sans-serif;
}
p {
  text-align: center;
}
</style>
