<template>
  <div>
    <form class='form'>
      <div class='form-group'>
        <label>Email Address:</label>
        <input v-model='email' class='form-control' type='email'>
      </div>
      <div class='form-group'>
        <label>Old Password:</label>
        <input v-model='password' class='form-control' type='password'>
      </div>
      <div class='form-group'>
        <label>New Password:</label>
        <input v-model='newPassword' class='form-control' type='password'>
      </div>
      <button type='submit'>Save</button>
    </form>
  </div>
</template>

<script>
  export default {
    data() {
      return {
        email: '',
        password: '',
        newPassword: ''
      }
    }
  }
</script>

<style scoped>
  /* mobile */
  form {
    width: 250px;
  }
  .form-control {
    width: 100%;
  }

  /* desktop */
  @media (min-width: 1000px) {
    .form-control {
      width: 200px;
    }
  }
</style>
