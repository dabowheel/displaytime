import Vue from 'vue'
import VueRouter from 'vue-router'
import App from './App.vue'
import Settings from './Settings.vue'

Vue.use(VueRouter)
const routes = [
  {
    path: 'settings',
    component: Settings
  }
];
const router = new VueRouter({routes});

new Vue({
  el: '#app',
  router,
  render: h => h(App)
})
