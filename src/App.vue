<template>
  <div id="app">
    <div id="toolbar" class="toolbar">
      <div id="toolbar-links" class="toolbar-links">
        <a v-show="!openSettings" id="signup" @click.prevent="clickSignup" href="#" class="btn toolbar-link click-link" title="sign up">Sign up</a>
        <a v-show="!openSettings" id="login" @click.prevent="clickLogin" href="#" class="btn toolbar-link click-link" title="login">Login</a>
        <a v-show="!openSettings" v-on:click.prevent="clickOpenSettings" href="#" class="btn toolbar-link" title="show settings"><i class="fa fa-cog fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
        <a v-show="openSettings" v-on:click.prevent="clickCloseSettings" href="#" class="btn toolbar-link" title="back"><i class="fa fa-times fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
      </div>
    </div>
    <div class="headers">
      <span v-show="openSettings" id="settings-header" class="toolbar-link">Settings</span>
    </div>
    <p v-show="!openSettings" id="datetime"><big>{{ datetimeString }}</big></p>
   <settings v-show="openSettings" v-bind:datetime-format="datetimeFormat" v-bind:datetimeString="datetimeString" v-on:setDatetimeFormat="handleDatetimeFormat">
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
      this.openSettings = true;
    },
    clickCloseSettings() {
      this.openSettings = false;
    },
    handleDatetimeFormat(newFormat) {
      this.datetimeFormat = newFormat;
    },
    clickSignup () {

    },
    clickLogin () {

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
/*
  Default to mobile
*/

body {
  font-family: Helvetica, sans-serif;
  margin: 0;
  padding: 0;
}
.toolbar-links {
  text-align: right;
}
.headers {
  text-align: center;
}
#datetime {
  text-align: center;
  top: 50%;
}
.toolbar-link:link, .toolbar-link:visited {
  text-decoration: none;
}
.toolbar-link:hover, .toolbar-link:active {
  text-decoration: underline;
}
.toolbar-link:hover, .toolbar-link:active {
  color: black;
}
.toolbar-link {
    color: #444444;
}

.toolbar {
  margin-left: 1em;
  margin-right: 1em;
}
.click-link {
  font-height: 2em;
  margin-left: .5em;
  margin-right: .5em;
}
.click-icon {
  font-size: 2em;
}
#settings-header {
  font-size: 2em;
}

/*
  A large screen
*/
@media (min-width: 1000px) {
  /* Font Size */
  .click-icon {
    font-size: 2em;
  }
  #date-time-field {
    width: 200px;
  }
}
</style>
