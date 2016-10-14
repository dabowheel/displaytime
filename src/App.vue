<template>
  <div id="app">
    <a v-on:click.prevent="settings" href="#" class="btn"><i class="fa fa-cog" aria-hidden="true"></i></a>
    <p id="date"><big>{{time.format("dddd MMMM Do YYYY")}}</big></p>
    <p id="time"><big>{{time.format("hh:mm")}}</big></p>
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
  },
  methods: {
    settings () {
      console.log("settings");
      return false;
    }
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
