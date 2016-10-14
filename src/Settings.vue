<template>
<div>
  <a v-on:click.prevent="settings" href="#" class="btn"><i class="fa fa-cog" aria-hidden="true"></i></a>
  <div id="settings"></div>
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
.fill {
  position: fixed;
  top: 0;
  left: 0;
  height: 100%;
  width: 100%;
  background: white;
}
</style>