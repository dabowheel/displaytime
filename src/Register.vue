<template>
    <div id="register">
        <label>Email Address:</label>
        <input type='text' v-model='email'/>
        <label>Password:</label>
        <input type='password' v-model='password'/>
    </div>
</template>
<script>
    export default {
        name: 'Register'
    };
</script>
<style>
</style>