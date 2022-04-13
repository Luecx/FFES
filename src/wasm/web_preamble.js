(() => {
    let quit = false;
    let n_nodes = 0;
    let is_running = false;
    let mode = 0;

    function transferToHeap(arr) {
        const floatArray = toFloatArr(arr);
        let heapSpace = Module._malloc(floatArray.length * floatArray.BYTES_PER_ELEMENT);
        Module.HEAPF32.set(floatArray, heapSpace >> 2);
        return heapSpace;

        function toFloatArr(arr) {
            const res = new Float32Array(arr.length);
            for (let i = 0; i < arr.length; i++) res[i] = arr[i];
            return res
        }
    }

    function getFloatArrayFromFunctionCall(func_name, size, it) {
        var res_ptr = Module._malloc(size * 4);
        Module.ccall(func_name, null, ["number", "number"], [res_ptr, it]);
        var view = Module.HEAPF32.subarray(res_ptr >> 2, (res_ptr >> 2) + size);
        let res = new Float32Array(view);
        Module._free(res_ptr);
        return res
    }

    const startAnalysis = command => {
        let n_elems = command["element_count"];
        let node_coords = command["node_coords"];
        let elem_node_ids = command["element_ids"];
        let material = command["material"];
        let restricted = command["restricted"];
        let displacement = command["displaced"];
        let loads = command["forces"];
        let nodes_per_element = elem_node_ids.length / n_elems;
        n_nodes = node_coords.length / 2;
        let _node_coords = transferToHeap(node_coords);
        let _elem_node_ids = transferToHeap(elem_node_ids);
        let _material = transferToHeap(material);
        let _restricted = transferToHeap(restricted);
        let _displacement = transferToHeap(displacement);
        let _loads = transferToHeap(loads);
        is_running = true;
        mode = 0;
        console.log(command);
        console.log("started analysis");
        Module.ccall("wasm_analysis", null, ["number", "number", "number", "number", "number", "number", "number", "number", "number"], [n_nodes, n_elems, _node_coords, _elem_node_ids, nodes_per_element, _material, _restricted, _displacement, _loads]);
        Module._free(_node_coords);
        Module._free(_elem_node_ids);
        Module._free(_material);
        Module._free(_restricted);
        Module._free(_displacement);
        Module._free(_loads)
    };
    const startTopo = command => {
        let target_density = command["target_density"];
        let simp_exponent = command["penalty_factor"];
        let r_min = command["filter_radius"];
        let min_density = command["min_density"];
        let move_limit = command["move_limit"];
        let max_iterations = command["max_iterations"];

        let n_elems = command["element_count"];
        let node_coords = command["node_coords"];
        let elem_node_ids = command["element_ids"];
        let material = command["material"];
        let restricted = command["restricted"];
        let displacement = command["displaced"];
        let loads = command["forces"];
        let nodes_per_element = elem_node_ids.length / n_elems;
        n_nodes = node_coords.length / 2;
        let _node_coords = transferToHeap(node_coords);
        let _elem_node_ids = transferToHeap(elem_node_ids);
        let _material = transferToHeap(material);
        let _restricted = transferToHeap(restricted);
        let _displacement = transferToHeap(displacement);
        let _loads = transferToHeap(loads);
        is_running = true;
        mode = 1;
        console.log(command);
        console.log("started topo");
        Module.ccall("wasm_topo", null, [
            "number", "number", "number", "number", "number",
            "number", "number", "number", "number", "number",
            "number", "number", "number", "number", "number"], [
                n_nodes, n_elems, _node_coords, _elem_node_ids, nodes_per_element,
            _material, _restricted, _displacement, _loads, target_density,
            simp_exponent, r_min, min_density, move_limit, max_iterations]);
        Module._free(_node_coords);
        Module._free(_elem_node_ids);
        Module._free(_material);
        Module._free(_restricted);
        Module._free(_displacement);
        Module._free(_loads)
    };
    const processCommand = command => {
        if (quit) {
            return
        }
        if (command === "exit" || command === "quit") {
            quit = true;
            PThread.terminateAllThreads();
            return
        }
    };
    const processAnalysisOutput = output => {
        if (output.startsWith("Attempting to solve matrix:")) {
            notify("analysis", [output])
        }
        if (output.startsWith("Decomposition finished;")) {
            notify("analysis", [output])
        }
        if (output.startsWith("Solving finished;")) {
            notify("analysis", [output])
        }
        if (output.startsWith("Finished computation")) {
            let it = 0;
            let disp_x = getFloatArrayFromFunctionCall("wasm_get_displacement_x", n_nodes, it);
            let disp_y = getFloatArrayFromFunctionCall("wasm_get_displacement_y", n_nodes, it);
            let disp_xy = getFloatArrayFromFunctionCall("wasm_get_displacement_xy", n_nodes, it);
            let stress_x = getFloatArrayFromFunctionCall("wasm_get_stress_x", n_nodes, it);
            let stress_y = getFloatArrayFromFunctionCall("wasm_get_stress_y", n_nodes, it);
            let stress_xy = getFloatArrayFromFunctionCall("wasm_get_stress_xy", n_nodes, it);
            let stress_mis = getFloatArrayFromFunctionCall("wasm_get_stress_mises", n_nodes, it);
            notify("analysis", [output, {
                "Displacement X": disp_x,
                "Displacement Y": disp_y,
                "Displacement": disp_xy,
                "Stress X": stress_x,
                "Stress Y": stress_y,
                "Stress XY": stress_xy,
                "Mises Stress": stress_mis
            }]);
            is_running = false
        }
    };
    const processTopoOutput = output => {
        if (output.match("^Iteration [0-9]+ finished")) {
            let it = parseInt(output.split(" ")[1]);
            let density = getFloatArrayFromFunctionCall("wasm_get_density", n_nodes, it);
            notify("topo", [output, {
                "Density": density}])
        }
        if (output.startsWith("Finished computation")){
            notify("topo", [output, {}])
            is_running = false
        }
    };
    const processSolverOutput = output => {
        if (mode === 0) {
            processAnalysisOutput(output)
        }
        if (mode === 1) {
            processTopoOutput(output)
        }
    };
    const listeners = {};
    const notify = (event, message) => {
        listeners[event].forEach(l => {
            l(message)
        })
    };
    Module.addMessageListener = ((event, l) => {
        if (!(event in listeners)) {
            listeners[event] = []
        }
        listeners[event].push(l)
    });
    Module.print = (s => processSolverOutput(s));
    Module.postMessage = (command => {
        if (!is_running) {
            processCommand(command)
        }
    });
    Module.startAnalysis = (values => {
        if (!is_running) {
            startAnalysis(values)
        }
    });
    Module.startTopo = (values => {
        if (!is_running) {
            startTopo(values)
        }
    });
    Module.postRun = (() => {
    })
})();