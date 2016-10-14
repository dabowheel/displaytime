<template>
<div>
  <a v-on:click.prevent="settings" href="#" class="btn"><i class="fa fa-cog" aria-hidden="true"></i></a>
  <div id="settings" class="nodisplay">
    <a v-on:click.prevent="closeSettings" class="btn" href="#"><i class="fa fa-arrow-left" aria-hidden="true"></i></a>
    <p id="settings-header"><big>Settings</big></p>
  </div>
</div>
</template>

<script>
export default {
  methods: {
    settings () {
      document.getElementById("settings").className = "fill";
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
</style>