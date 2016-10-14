<template>
  <div id="app">
    <settings v-bind:datetime="datetime" v-bind:datetime-format="datetimeFormat" v-bind:datetime-string="datetimeString" v-on:setDatetimeFormat="handleDatetimeFormat">
    </settings>
    <p id="date"><big>{{ datetimeString }}</big></p>
  </div>
</template>

<script>
import moment from 'moment';
import Settings from './Settings.vue';

export default {
  data () {
    var datetime = moment();
    var datetimeFormat = localStorage.datetimeFormat ? localStorage.datetimeFormat: "llll";
    return {
      datetime: datetime,
      datetimeFormat: datetimeFormat,
      datetimeString: datetime.format(datetimeFormat),
    }
  },
  watch: {
    datetime: function (newDatetime) {
      this.datetimeString = newDatetime.format(this.datetimeFormat);
    },
    datetimeFormat: function (newFormat) {
      this.datetimeString = this.datetime.format(newFormat);
      localStorage.datetimeFormat = newFormat;
    }
  },
  methods: {
    handleDatetimeFormat(newFormat) {
      this.datetimeFormat = newFormat;
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
  font-size: 4em;
  margin: 0;
  padding: 0;
}
@media (min-width: 1000px) {
  body {
    font-size: 1em;
  }
}
#date,#time {
  text-align: center;
}
</style>
