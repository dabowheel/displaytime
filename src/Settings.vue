<template>
<div>
  <a v-on:click.prevent="settings" href="#" class="btn" title="show settings"><i class="fa fa-cog fa-2x settings-nav" aria-hidden="true"></i></a>
  <div id="settings" class="nodisplay">
    <a v-on:click.prevent="closeSettings" class="btn" href="#" title="back"><i class="fa fa-arrow-left fa-2x settings-nav" aria-hidden="true"></i></a>
    <p id="date-formatted"><big>{{ dateFormatted.replace(" ", "&nbsp") }}</big></p>
    <p id="settings-header"><big>Date Settings</big></p>
    <p id="date-format-field"><input type="text" v-model="myDateFormat" placeholder="date format"/></p>
    <p id="guide">Guide</p>
    <table border="1">
      <thead>
        <tr>
          <th>Format</th><th>Example</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>YY</td><td>70 71 ... 29 30</td>
        </tr>
        <tr>
          <td>YYY</td><td>1970 1971 ... 2029 2030</td>
        </tr>
        <tr>
          <td>Y</td><td>1970 1971 ... 9999 +10000 +10001 <strong>Note</strong>: This complies with the ISO 8601 standard for dates past the year 9999</td>
        </tr>
      </tbody>
    </table> 
  </div>
</div>
</template>

<script>
export default {
  data () {
    return {
      myDateFormat: this.dateFormat,
      myTimeFormat: this.timeFormat
    }
  },
  props: [
    "datetime",
    "dateFormat",
    "timeFormat",
    "dateFormatted",
    "timeFormatted"
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
    myDateFormat (newFormat) {
      this.$emit("setDateFormat", newFormat);
    }
  }
}
</script>

<style>
#settings-header, #date-formatted, #date-format-field, #guide {
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
</style>