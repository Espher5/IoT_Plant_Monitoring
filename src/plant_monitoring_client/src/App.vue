<template>
  <app-header />
  <app-main />
  <router-view />
</template>

<script>
import AppHeader from './components/app-header.vue';
import AppMain from './components/app-main.vue';

export default {
  name: 'App',
  components: {
    AppHeader,
    AppMain
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
