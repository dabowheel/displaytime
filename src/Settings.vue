<template>
<div id="settings" class="nodisplay">
  <div id="datetime-format-label">Date/Time format:</div>
  <div id="datetime-format-field"><input id="datetime-format-input" type="text" v-model="myDatetimeFormat" placeholder="date/time format"/></div>
  <div id="guide">Guide</div>
  <table id="guide-table" border="1">
    <thead>
      <tr>
        <th class="guide-header">Format</th><th class="guide-header">Example</th>
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
  <p id="more-details">For details on formats see the <a href="http://momentjs.com/docs/#/displaying/">Moment.js</a> documentation.</p>
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
    "open",
    "datetimeFormat",
  ],
  methods: {
  },
  watch: {
    open () {
      console.log("open " + this.open)
      document.getElementById("settings").style.display = (this.open) ? "inline": "none";
    },
    myDatetimeFormat (newFormat) {
      this.$emit("setDatetimeFormat", newFormat);
    }
  }
}
</script>

<style>
#guide {
  margin-top: 15px;
  margin-bottom: 5px;
}
.guide-header {
  font-size: 0.6em;
}
#datetime-format-label, #datetime-string, #datetime-format-field, #guide, #more-details {
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

@media (min-width: 1000px) {
  #datetime-format-input {
    width: 200px;
  }
}
</style>
