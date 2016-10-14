<template>
<div>
  <a v-on:click.prevent="settings" href="#" class="btn" title="show settings"><i class="fa fa-cog fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
  <div id="settings" class="nodisplay">
    <a v-on:click.prevent="closeSettings" class="btn" href="#" title="back"><i class="fa fa-arrow-left fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
    <p id="datetime-string"><big>{{ datetimeString.replace(" ", "&nbsp") }}</big></p>
    <p id="settings-header"><big>Date/Time Settings</big></p>
    <p id="datetime-format-field"><input type="text" v-model="myDatetimeFormat" placeholder="date/time format"/></p>
    <p id="guide">Guide</p>
    <table id="guide-table" border="1">
      <thead>
        <tr>
          <th>Format</th><th>Example</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>LLL</td><td>September 4 1986 8:30 PM</td>
        </tr>
        <tr>
          <td>lll</td><td>Sep 4 1986 8:30 PM</td>
        </tr>
        <tr>
          <td>LLLL</td><td>Thursday, September 4 1986 8:30 PM</td>
        </tr>
        <tr>
          <td>llll</td><td>Thu, Sep 4 1986 8:30 PM</td>
        </tr>
      </tbody>
    </table>
    <p id="more-details">For details on formats see <a href="http://momentjs.com/docs/#/displaying/">Moment.js</a> documentation.</p>
  </div>
</div>
</template>

<script>
export default {
  data () {
    return {
      myDatetimeFormat: this.datetimeFormat,
    }
  },
  props: [
    "datetime",
    "datetimeFormat",
    "datetimeString"
  ],
  methods: {
    settings () {
      document.getElementById("settings").className = "fill";
      return false;
    },
    closeSettings() {
      document.getElementById("settings").className = "nodisplay";
      return false;
    }
  },
  watch: {
    myDatetimeFormat (newFormat) {
      this.$emit("setDatetimeFormat", newFormat);
    }
  }
}
</script>

<style>
#settings-header, #datetime-string, #datetime-format-field, #guide, #more-details {
  text-align: center;
}
.fill {
  position: fixed;
  top: 0;
  left: 0;
  height: 100%;
  width: 100%;
  background: white;
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
.click-icon {
  font-size: 2em;
}

@media (min-width: 1000px) {
  .click-icon {
    font-size: 2em;
  }
}
</style>
