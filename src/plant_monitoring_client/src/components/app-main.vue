<template> 
    <div class="data-buttons">
        <menu-item v-for="item in items" :key="item.topic" :buttonLabel="item.name"></menu-item>
    </div> 
    <div id="chart-area">
        <sensor-chart  v-for="item in items" :key="item.topic" :topic="item.topic" :name="item.name"/>
    </div>  
</template>

<script>
import menuItem from './menu-item.vue'
import SensorChart from './sensor-chart.vue';

export default {
    components: { menuItem, SensorChart },
    props: {
        items: {
            type: Array,
            default: null
        }
    }
}
</script>
