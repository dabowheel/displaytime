<template>
  <div>
    {{ lastError }}
  </div>
</template>

<script>
  export default {
    computed: {
      lastError() {
        return this.$store.state.lastError
      }
    }
  }
</script>

<style>
</style>