<template>
  <div id="settings">
    <a v-on:click.prevent="settings" href="#" class="btn"><i class="fa fa-cog" aria-hidden="true"></i></a>
  </div>
</template>

<script>
export default {
  methods: {
    settings () {
      console.log("settings");
      return false;
    }
  }
}
</script>

<style>
</style>