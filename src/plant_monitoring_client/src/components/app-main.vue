<template> 
    <div class = "data-buttons">
        <menu-item v-for="item in items" :key="item.id" :buttonLabel="item.label"></menu-item>
    </div>   
</template>

<script>
import menuItem from './menu-item.vue'
import { ref } from 'vue';

export default {
    components: { menuItem },
    setup() {
        const items = ref([
            {id: 0, label: 'Moisture'},
            {id: 1, label: 'Water level'},
            {id: 2, label: 'Light level'}, 
            {id: 3, label: 'Temperature'},
            {id: 4, label: 'Humidity'}
        ]);

        return {
            items
        };
    }
}
</script>
