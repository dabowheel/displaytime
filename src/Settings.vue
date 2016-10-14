<template>
<div>
  <a v-on:click.prevent="settings" href="#" class="btn" title="show settings"><i class="fa fa-cog fa-2x settings-nav" aria-hidden="true"></i></a>
  <div id="settings" class="nodisplay">
    <a v-on:click.prevent="closeSettings" class="btn" href="#" title="back"><i class="fa fa-arrow-left fa-2x settings-nav" aria-hidden="true"></i></a>
    <p id="settings-header"><big>Settings</big></p>
    <label>Date format:</label>
    <input type="text" v-bind:value="dateFormat" />
  </div>
</div>
</template>

<script>
export default {
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
  }
}
</script>

<style>
#settings-header {
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