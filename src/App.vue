<template>
  <div id="app">
    <a id="open-settings" v-on:click.prevent="clickOpenSettings" href="#" class="btn" title="show settings"><i class="fa fa-cog fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
    <a id="close-settings" v-on:click.prevent="clickCloseSettings" href="#" class="btn" title="back"><i class="fa fa-arrow-left fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
    <p id="date"><big>{{ datetimeString }}</big></p>
   <settings v-bind:open="openSettings" v-bind:datetime-format="datetimeFormat" v-on:setDatetimeFormat="handleDatetimeFormat">
    </settings>
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
      openSettings: false
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
    clickOpenSettings () {
      document.getElementById("open-settings").style.display = "none";
      document.getElementById("close-settings").style.display = "inline";
      this.openSettings = true;
      return false;
    },
    clickCloseSettings() {
      document.getElementById("close-settings").style.display = "none";
      document.getElementById("open-settings").style.display = "inline";
      this.openSettings = false;
      return false;
    },
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
  font-size: 2em;
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
#close-settings {
  display: none;
}
</style>
