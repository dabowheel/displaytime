<template>
  <div id="app">
    <div id="toolbar" class="toolbar">
      <div id="toolbar-links" class="toolbar-links">
        <a id="signup" @click.prevent="clickSignup" href="#" class="btn toolbar-link click-link" title="sign up">Sign up</a>
        <a id="login" @click.prevent="clickLogin" href="#" class="btn toolbar-link click-link" title="login">Login</a>
        <a @click.prevent="clickOpenSettings" href="#" class="btn toolbar-link" title="show settings"><i class="fa fa-cog fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
      </div>
    </div>
    <p id="datetime">{{ datetimeString }}</p>
  </div>
</template>

<script>
import moment from 'moment';
import page from 'page';

export default {
  data () {
    var datetime = moment();
    var datetimeFormat = localStorage.datetimeFormat ? localStorage.datetimeFormat: "llll";
    return {
      datetime: datetime,
      datetimeFormat: datetimeFormat,
      datetimeString: datetime.format(datetimeFormat)
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
      page('/settings');
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
  font-size: 2em;
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
  margin-right: 1em;
}
.click-link {
  font-height: 2em;
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
