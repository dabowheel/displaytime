<template>
<div id="settings">
  <div id="toolbar" class="toolbar">
    <div id="toolbar-links" class="toolbar-links">
      <span class="header">Settings</span>
      <a @click.prevent="clickCloseSettings" href="#" class="btn toolbar-link" title="show settings"><i class="fa fa-times fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
    </div>
  </div>
  <div id="datetime-format-label">Date/Time format:</div>
  <div id="datetime-format-field"><input id="datetime-format-input" class="format-string" type="text" v-model="datetimeFormat" placeholder="date/time format"/></div>
  <div class="displayfield">{{datetimeString}}</div>
  <div id="guide">Guide</div>
  <table id="guide-table" border="1">
    <thead>
      <tr>
        <th class="guide-header">Format</th><th class="guide-header">Example</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td class="format-string">LLL</td><td>September 4 1986 8:30 PM</td>
      </tr>
      <tr>
        <td class="format-string">lll</td><td>Sep 4 1986 8:30 PM</td>
      </tr>
      <tr>
        <td class="format-string">LLLL</td><td>Thursday, September 4 1986 8:30 PM</td>
      </tr>
      <tr>
        <td class="format-string">llll</td><td>Thu, Sep 4 1986 8:30 PM</td>
      </tr>
    </tbody>
  </table>
  <p id="more-details">For details on formats see the <a href="http://momentjs.com/docs/#/displaying/">Moment.js</a> documentation.</p>
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
  created() {
    setInterval(function () {
      this.datetime = moment();
    }.bind(this), 1000);
  },
  methods: {
    clickCloseSettings() {
      page('/');
    }
  }
}
</script>

<style>
.header {
  font-size: 1.5em;
}
.format-string, input.format-string{
  font-family: Bitstream Vera Sans Mono;
}
#guide {
  margin-top: 1em;
  margin-bottom: .5em;
}
#datetime-format-label, #datetime-string, #datetime-format-field, #guide, #more-details, .label, .displayfield {
  text-align: center;
}
#datetime-format-input {
  width: 100%;
  box-sizing: border-box;
}
.nodisplay {
  display: none;
}
.settings-nav {
  margin: 5px;
}
table {
  border-collapse: collapse;
  border-style: none;
  margin-left: auto;
  margin-right: auto;
}
th {
  background-color: lightblue;
  color: white;
}
td,th {
  padding: 0;
  margin: 0;
  border-color: #aaaaaa;
  padding-left: 5px;
  padding-right: 5px;
}
th {
  border-color: green;
}
td {
  border-left-style: none;
  border-right-style: none;
  border-bottom-style: none;
}
.displayfield {
  font-size: 2em;
}

@media (min-width: 1000px) {
  #datetime-format-input {
    width: 200px;
  }
}
</style>
