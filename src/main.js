import Vue from 'vue'
import App from './App.vue'
import page from 'page'
import Settings from './Settings.vue'

const app = new Vue({
  el: '#app',
  data: {
    ViewComponent: App
  },
  render (h) {
    return h(this.ViewComponent)
  }
})

page('/', function () {
  app.ViewComponent = App
})

page('/settings', function () {
  app.ViewComponent = Settings
})
