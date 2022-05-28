<template>
  <app-header />
  <app-main :items="items"/>
</template>

<script>
import AppHeader from './components/app-header.vue';
import AppMain from './components/app-main.vue';
import { ref } from 'vue';

export default {
    name: 'App',
    components: {
        AppHeader,
        AppMain
    },
    data() {
        const items = ref([
            {topic: '/iot/moisture', name: 'moisture'},
            {topic: '/iot/water', name: 'water level'},
            {topic: '/iot/light', name: 'light level'}, 
            {topic: '/iot/temperature', name: 'air temperature'},
            {topic: '/iot/humidity', name: 'air humidity'}
        ]);

        return {
            items
        };
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
